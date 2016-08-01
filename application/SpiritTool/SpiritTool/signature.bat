@echo off
jarsigner -keystore D:\ghost\bin\spirit.keystore -storepass spirit001 -keypass spirit007 -signedjar D:\ghost\bin\testkey.apk D:\ghost\bin\test.apk test
copy "D:\ghost\bin\testkey.apk" "D:\testSpiritTool\test3.apk"
@echo on
