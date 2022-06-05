$ffmpegName = "ffmpeg-n5.0-latest-win64-lgpl-shared-5.0";
Invoke-WebRequest -Method Get -Uri "https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/$ffmpegName.zip" -OutFile "ffmpeg.zip";
Expand-Archive -Path "ffmpeg.zip" .;
Copy-Item -Filter *.dll -Path "$ffmpegName\bin" -Destination "../../build/ffmpeg" -Recurse;
Remove-Item -Path "ffmpeg.zip";
Remove-Item -Path "$ffmpegName" -Recurse;
