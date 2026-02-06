Invoke-WebRequest -Method Get -Uri "https://go.microsoft.com/fwlink/?linkid=2349110" -OutFile "winsdksetup.exe";
Start-Process -Path "winsdksetup.exe" -ArgumentList "/repair", "/q", "/norestart", "/ceip off" -Wait -Passthru;
Remove-Item -Path "winsdksetup.exe";
