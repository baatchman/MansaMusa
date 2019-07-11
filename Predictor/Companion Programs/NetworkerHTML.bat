:main
del ".\NT3.Save\MMap\Scary_Solstice\*.*"
pause
NetworkHTML.exe
copy ".\NT3.Save\MMap\Scary_Solstice\spooky.html" ".\spooky.html"
goto main