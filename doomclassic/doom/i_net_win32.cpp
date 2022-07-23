/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "Precompiled.h"
#include "globaldata.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>

#include <errno.h>

#include "i_system.h"
#include "d_event.h"
#include "d_net.h"
#include "m_argv.h"

#include "doomstat.h"

#include "i_net.h"

#include "doomlib.h"
#include <WS2tcpip.h>

void	NetSend (void);
qboolean NetListen (void);

typedef ULONG in_addr_t;
extern bool globalNetworking;

namespace {
	bool IsValidSocket( SOCKET socketDescriptor );
	int GetLastSocketError();



	/*
	========================
	Returns true if the socket is valid. I made this function to help abstract the differences
	between WinSock (used on Xbox) and BSD sockets, which the PS3 follows more closely.
	========================
	*/
	bool IsValidSocket( SOCKET socketDescriptor ) { //GK:Make proper check up using winsocks
		int optval;
		int optlen = sizeof(int);
		if (getsockopt(socketDescriptor, SOL_SOCKET, SO_ERROR, (char *)&optval, &optlen) != 0) {
			return false;
		}
		return true;
	}

	/*
	========================
	Returns the last error reported by the platform's socket library.
	========================
	*/
	int GetLastSocketError() {
		return WSAGetLastError();
	}
}

//
// NETWORKING
//
int	DOOMPORT = 6666; //GK:Use this port because why not?	// DHM - Nerve :: On original XBox, ports 1000 - 1255 saved you a byte on every packet.  360 too?
WSADATA windata; //GK:winsock related stuff
unsigned long GetServerIP() {
	return ::g->sendaddress[::g->doomcom.consoleplayer].sin_addr.s_addr;
	//return 0;
}

void	(*netget) (void);
void	(*netsend) (void);


//
// UDPsocket
//
SOCKET UDPsocket (void) //GK:return SOCKET instead of int
{
	SOCKET	s;

	// allocate a socket
	s = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if ( !IsValidSocket( s ) ) {
		int err = GetLastSocketError();
		//GK:Show proper error message
		char msgbuf[256];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), msgbuf, sizeof(msgbuf), NULL);
		I_Error( "can't create socket, error %s", msgbuf );
	}

	return s;
	//return 0;
}


//
// BindToLocalPort
//
void BindToLocalPort( SOCKET	s, int	port )//GK:Restored source code from the vanilla DOOM source code
{
	int			v;
	struct sockaddr_in	address;

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = port;

	v = bind(s, (SOCKADDR *)&address, sizeof(address));
	if (v == SOCKET_ERROR) {
		int err = GetLastSocketError();
		char msgbuf[256];
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), msgbuf, sizeof(msgbuf), NULL);
		I_Error("BindToPort: bind: %s", msgbuf);
	}
}


//
// PacketSend
//
void PacketSend (void)//GK:Restored source code from the vanilla DOOM source code
{
	int		c;
	doomdata_t	sw;

	// byte swap
	sw.checksum = htonl(::g->netbuffer->checksum);
	sw.player = ::g->netbuffer->player;
	sw.retransmitfrom = ::g->netbuffer->retransmitfrom;
	sw.starttic = ::g->netbuffer->starttic;
	sw.numtics = ::g->netbuffer->numtics;
	for (c = 0; c< ::g->netbuffer->numtics; c++)
	{
		sw.cmds[c].forwardmove = ::g->netbuffer->cmds[c].forwardmove;
		sw.cmds[c].sidemove = ::g->netbuffer->cmds[c].sidemove;
		sw.cmds[c].angleturn = htons(::g->netbuffer->cmds[c].angleturn);
		sw.cmds[c].consistancy = htons(::g->netbuffer->cmds[c].consistancy);
		//sw.cmds[c].chatchar = netbuffer->cmds[c].chatchar;
		sw.cmds[c].buttons = ::g->netbuffer->cmds[c].buttons;
	}

	//printf ("sending %i\n",gametic);		
	c = sendto(::g->sendsocket, (char *)&sw, ::g->doomcom.datalength
		, 0, (SOCKADDR *)&::g->sendaddress[::g->doomcom.remotenode]
		, sizeof(::g->sendaddress[::g->doomcom.remotenode]));

	if (c == SOCKET_ERROR) {
		int err = GetLastSocketError();
		char msgbuf[256];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), msgbuf, sizeof(msgbuf), NULL);
		I_Error("SendPacket error: %s", msgbuf);
	}
}


//
// PacketGet
//
void PacketGet (void)//GK:Restored source code from the vanilla DOOM source code
{
	int			i;
	int			c;
	struct sockaddr_in	fromaddress;
	int			fromlen;
	doomdata_t		sw;

	fromlen = sizeof(fromaddress);
	c = recvfrom(::g->insocket, (char*)&sw, sizeof(sw), 0
		, (struct sockaddr *)&fromaddress, &fromlen);
	//GK:do similar to vanilla check ups using winsock
	if (c == SOCKET_ERROR)
	{
		int err = GetLastSocketError();
		char msgbuf[256];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), msgbuf, sizeof(msgbuf), NULL);
		if (err != WSAEWOULDBLOCK)
			I_Error("GetPacket: %s", msgbuf);
		::g->doomcom.remotenode = -1;		// no packet
		return;
	}

	{
		static int first = 1;
		if (first)
			I_Printf("len=%d:p=[0x%x 0x%x] \n", c, *(int*)&sw, *((int*)&sw + 1));
		first = 0;
	}

	// find remote node number
	for (i = 0; i< ::g->doomcom.numnodes; i++)
		if (fromaddress.sin_addr.s_addr == ::g->sendaddress[i].sin_addr.s_addr)
			break;

	if (i == ::g->doomcom.numnodes)
	{
		// packet is not from one of the players (new game broadcast)
		::g->doomcom.remotenode = -1;		// no packet
		return;
	}

	::g->doomcom.remotenode = i;			// good packet from a game player
	::g->doomcom.datalength = c;

	// byte swap
	::g->netbuffer->checksum = ntohl(sw.checksum);
	::g->netbuffer->player = sw.player;
	::g->netbuffer->retransmitfrom = sw.retransmitfrom;
	::g->netbuffer->starttic = sw.starttic;
	::g->netbuffer->numtics = sw.numtics;

	for (c = 0; c < ::g->netbuffer->numtics; c++)
	{
		::g->netbuffer->cmds[c].forwardmove = sw.cmds[c].forwardmove;
		::g->netbuffer->cmds[c].sidemove = sw.cmds[c].sidemove;
		::g->netbuffer->cmds[c].angleturn = ntohs(sw.cmds[c].angleturn);
		::g->netbuffer->cmds[c].consistancy = ntohs(sw.cmds[c].consistancy);
		//netbuffer->cmds[c].chatchar = sw.cmds[c].chatchar;
		::g->netbuffer->cmds[c].buttons = sw.cmds[c].buttons;
	}
}

static int I_TrySetupNetwork(void) //GK:Manualy init winsock
{
	int r;
	r = WSAStartup(MAKEWORD(1, 1), &windata);
	if (r != NO_ERROR) {
		I_Printf("Winsock Error:%d\n", r);
		return 0;
	}
	else {
		return 1;
	}

}

//
// I_InitNetwork
//
void I_InitNetwork (void)
{
	//qboolean		trueval = true;
	int			i;
	int			p;
	//int a = 0;
	//    struct hostent*	hostentry;	// host information entry

	memset (&::g->doomcom, 0, sizeof(::g->doomcom) );

	// set up for network
	i = M_CheckParm ("-dup");
	if (i && i< ::g->myargc-1)
	{
		::g->doomcom.ticdup = ::g->myargv[i+1][0]-'0';
		if (::g->doomcom.ticdup < 1)
			::g->doomcom.ticdup = 1;
		if (::g->doomcom.ticdup > 9)
			::g->doomcom.ticdup = 9;
	}
	else
		::g->doomcom.ticdup = 1;

	if (M_CheckParm ("-extratic"))
		::g->doomcom.extratics = 1;
	else
		::g->doomcom.extratics = 0;

	p = M_CheckParm ("-port");
	if (p && p < ::g->myargc-1)
	{
		DOOMPORT = atoi (::g->myargv[p+1]);
		I_Printf ("using alternate port %i\n",DOOMPORT);
	}

	// parse network game options,
	//  -net <::g->consoleplayer> <host> <host> ...
	i = M_CheckParm ("-net");
	if (!i || !I_TrySetupNetwork())
	{
		// single player game
		::g->netgame = false;
		::g->doomcom.id = DOOMCOM_ID;
		::g->doomcom.numplayers = ::g->doomcom.numnodes = 1;
		::g->doomcom.deathmatch = false;
		::g->doomcom.consoleplayer = 0;
		return;
	}

	netsend = PacketSend;
	netget = PacketGet;

#ifdef ID_ENABLE_DOOM_CLASSIC_NETWORKING
	::g->netgame = true;

	{
		++i; // skip the '-net'
		::g->doomcom.numnodes = 0;
		::g->doomcom.consoleplayer = atoi( ::g->myargv[i] );
		// skip the console number
		++i;
		::g->doomcom.numnodes = 0;
		for (; i < ::g->myargc; ++i)
		{
			//GK: Make sure it doesn't getting other parameters as ip addresses
			if (::g->myargv[i][0] == '-' || ::g->myargv[i][0] == '+')
				break;

			::g->sendaddress[::g->doomcom.numnodes].sin_family = AF_INET;
			::g->sendaddress[::g->doomcom.numnodes].sin_port = htons(DOOMPORT);
			
			// Pull out the port number.
			const std::string ipAddressWithPort( ::g->myargv[i] );
			const std::size_t colonPosition = ipAddressWithPort.find_last_of(':');
			std::string ipOnly;

			if( colonPosition != std::string::npos && colonPosition + 1 < ipAddressWithPort.size() ) {
				const std::string portOnly( ipAddressWithPort.substr( colonPosition + 1 ) );

				::g->sendaddress[::g->doomcom.numnodes].sin_port = htons( atoi( portOnly.c_str() ) );

				ipOnly = ipAddressWithPort.substr( 0, colonPosition );
			} else {
				// Assume the address doesn't include a port.
				ipOnly = ipAddressWithPort;
			}

			in_addr_t ipAddress;
			inet_pton(AF_INET, ipOnly.c_str(),  &ipAddress);

			if ( ipAddress == INADDR_NONE ) {
				I_Error( "Invalid IP Address: %s\n", ipOnly.c_str() );
				session->QuitMatch();
				common->Dialog().AddDialog( GDM_OPPONENT_CONNECTION_LOST, DIALOG_ACCEPT, NULL, NULL, false );
			}
			::g->sendaddress[::g->doomcom.numnodes].sin_addr.s_addr = ipAddress;
			::g->doomcom.numnodes++;
		}
		
		::g->doomcom.id = DOOMCOM_ID;
		::g->doomcom.numplayers = ::g->doomcom.numnodes;
		//GK:Init and bind sockets
		::g->insocket = UDPsocket();
		int socbuff = 1;
		setsockopt(::g->insocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&socbuff, sizeof(socbuff)); //GK:Does it work ???
		BindToLocalPort(::g->insocket, htons(DOOMPORT));
		unsigned long nonblocking = 1;
		int r=ioctlsocket(::g->insocket, FIONBIO,&nonblocking);
		if (r == SOCKET_ERROR) {
			int err = GetLastSocketError();
			char msgbuf[256];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), msgbuf, sizeof(msgbuf), NULL);
			I_Error("Socket Error:%d\n", msgbuf);
		}
		::g->sendsocket = UDPsocket();

	}

	if ( globalNetworking ) {
		// Setup sockets
		::g->insocket = UDPsocket ();
		BindToLocalPort (::g->insocket,htons(DOOMPORT));
		
		// PS3 call to enable non-blocking mode
		unsigned long nonblocking = 1; // Non-zero is nonblocking mode.
		ioctlsocket(::g->insocket, FIONBIO, &nonblocking); //GK:set this mode properly

		::g->sendsocket = UDPsocket ();

		I_Printf( "[+] Setting up sockets for player %d\n", DoomLib::GetPlayer() );
	}
#endif
}

// DHM - Nerve
void I_ShutdownNetwork() {
	
}

void I_NetCmd (void) //GK:Revie Netcode
{
	if (::g->doomcom.command == CMD_SEND)
	{
		netsend ();
	}
	else if (::g->doomcom.command == CMD_GET)
	{
		netget ();
	}
	else
		I_Error ("Bad net cmd: %i\n",::g->doomcom.command);
}

