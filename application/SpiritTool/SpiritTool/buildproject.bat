@echo off
javac -encoding UTF-8 -bootclasspath D:\android-sdk\platforms\android-7\android.jar -d D:\ghost\bin\classes D:\ghost\src\com\jqbar\test\my\*.java D:\ghost\gen\com\jqbar\test\my\R.java D:\ghost\src\my\global\*.java
D:\android-sdk\platform-tools\dx --dex --output=D:\ghost\bin\classes.dex D:\ghost\bin\classes
@echo on
