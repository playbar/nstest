@echo off
keytool -genkey -keystore D:\ghost\bin\spirit.keystore -keyalg RSA -validity 36500 -storepass spirit001 -alias test -keypass spirit007
@echo on
