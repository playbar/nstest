@echo off
mkdir "D:\ghost\src\com\jqbar\test\my"
move "D:\ghost\src\com\jqbar\test\nh\*java" "D:\ghost\src\com\jqbar\test\my"
rd "D:\ghost\src\com\jqbar\test\nh"
rd "D:\ghost\src\com\jqbar\test"
rd "D:\ghost\src\com\jqbar"
rd "D:\ghost\src\com"
copy "C:\Users\yock\Pictures\{4E89AFCC-1BEC-4F0B-910B-32B418B49373}¸±±¾.png" "D:\ghost\res\drawable\ic_launcher.png"
copy "C:\Users\yock\Pictures\{8BF9A8EF-E186-4ECA-9DAB-2B76A2ABC076}.png" "D:\ghost\res\drawable\img_a.png"
copy "C:\Users\yock\Pictures\{05415659-A961-4A73-906D-5B8F402AE2B5}.png" "D:\ghost\res\drawable\img_b.png"
copy "C:\Users\yock\Pictures\{83DADDDE-D9AB-4888-B4B9-5AE0A67DDE11}.gif" "D:\ghost\assets\game.gif"
del "D:\ghost\assets\ggclient.apk"
@echo on
