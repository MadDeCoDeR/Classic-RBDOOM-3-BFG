/*
Open Platform

Copyright(C) 2023 George Kalampokis

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files(the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

class OpenMultiplayer {
public:
	virtual void CreateLobby(int type, int maxplayers) = 0;
	virtual int CountLobbyUsers() = 0;
	virtual unsigned long long GetLobbyUserId(int index) = 0;
	virtual void JoinLobby(unsigned long long id) = 0;
	virtual void LeaveLobby() = 0;
	virtual bool SendPacket(unsigned long long userId, const void* data, unsigned int size) = 0;
	virtual bool hasAvailablePackets(unsigned int* size) = 0;
	virtual bool ReadPacket(void* data, int size, unsigned int* receivedSize, unsigned  long long* userId) = 0;
	virtual bool CloseConnection(unsigned long long userId) = 0;
};

OpenMultiplayer* getMultiplayerInstance(bool apiEnabled);
