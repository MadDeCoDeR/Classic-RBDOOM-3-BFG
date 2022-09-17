$ffmpegName = "ffmpeg-n5.1-latest-win64-lgpl-shared-5.1";
Invoke-WebRequest -Method Get -Uri "https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/$ffmpegName.zip" -OutFile "ffmpeg.zip";
Expand-Archive -Path "ffmpeg.zip" .;
Copy-Item -Filter *.dll -Path "$ffmpegName\bin" -Destination "../../build/ffmpeg" -Recurse;
Remove-Item -Path "ffmpeg.zip";
Remove-Item -Path "$ffmpegName" -Recurse;
