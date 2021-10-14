 # Лекция 1 

## Полезные ссылки

• C Programming — http://en.wikibooks.org/wiki/C_Programming
• Google C++ Code Style — https://google.github.io/styleguide/cppguide.html
• Code Style Guidelines | WebKit — https://webkit.org/code-style-guidelines/
• LLVM Coding Standards — LLVM5 documentation:
http://llvm.org/docs/CodingStandards.html
• Справка по языкам C / C++ — http://en.cppreference.com/w/
• The C++ Resources Network — http://www.cplusplus.com/
• Standard C++ — http://isocpp.org/
• Cpp Core Guidelines — https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
• More C++ Idioms — http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms
• Stack Overflow — http://stackoverflow.com/
• YouTube-канал конференций CppCon — https://www.youtube.com/user/CppCon
• YouTube-канал рабочей группы C++ —https://www.youtube.com/channel/UCJ9v015sPgEi0jJXe_zanjA   

## Директивы препроцессора 

Примеры директив:

- `#include` – замечается содержимым файла с исходным кодом, указанным в
  качестве параметра
- `#define` – вводит новые символические константы и макроопределения
  (обратная директива - #undef)
- #if, #ifdef, #ifndef, #else, #elif, #endif – позволяют организовывать
  включение/исключение фрагментов кода в зависимости от условия
- `#error` – возбуждение ошибки
- `#pragma` – выполнение операций, зависящих от реализации компилятора  

 **Куча** (heap) — контролируется программистом посредством
вызова, в том числе, функций malloc / free:

- располагается «выше» сегмента BSS;
- является общей для всех разделяемых библиотек и динамически
  загружаемых объектов (DLO, dynamically loaded objects) процесса  



# Cеминар 1 | Git



# Лекция 2 

```bash 
pwd  #print working directory
rm -r #удалить рекурсивно
cat b #содержимое файла
cp b b_copy #копирование из b в новый файл
mv b_copy c #переименует b_copy в c 
#проверить

echo 'hello' > c записывает в файл
echo 'hello' >> d добить в конец

wc d #кол-во 
2 4 22 #строк слов символов

# to do

cat d | wc-l #передаться то что получила команда в другую команду

```

приведение всегда к беззнаковым  -1 > 1

