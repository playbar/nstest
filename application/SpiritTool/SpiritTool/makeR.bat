@echo off
D:\android-sdk\platform-tools\aapt package -f -M D:\ghost\AndroidManifest.xml -S D:\ghost\res -A D:\ghost\assets -I D:\android-sdk\platforms\android-7\android.jar -F D:\ghost\bin\resources.ap_
D:\android-sdk\platform-tools\aapt package -f -m -J D:\ghost\gen -S D:\ghost\res -I D:\android-sdk\platforms\android-7\android.jar -M D:\ghost\AndroidManifest.xml
@echo on
