%qt5_path%\bin\lupdate.exe ".\\" "..\\QtPropertyBrowser" -ts modelexplorerplugin_ru.ts
%qt5_path%\bin\lupdate.exe ".\\" "..\\QtPropertyBrowser" -ts modelexplorerplugin_en.ts

%qt5_path%\bin\lrelease.exe modelexplorerplugin_ru.ts
%qt5_path%\bin\lrelease.exe modelexplorerplugin_en.ts