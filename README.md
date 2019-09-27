# CodeAndLink

http://www.codersblock.org/blog/multiplayer-fps-part-1

## raccourci visual studio  

CTRL+F7 -> rebuild only current cpp

Toujours les && et pas les || car compilateur peut cut avant.  

## Regex
	std::regex mregex("<(.*)/>");
	std::regex mregex("<(.*)>(.*)</(.*)>");
	std::regex mregex("<(.*)>");
	std::regex mregex("</(.*)>");
	std::regex mregex("(.+)=\"(.+)\"");

## Pour faire son IDE
https://docs.microsoft.com/fr-fr/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesstimes
https://docs.microsoft.com/en-us/windows/win32/psapi/collecting-memory-usage-information-for-a-process

## Faire une clée unique avec deux unsigned int (ou uint32 = pareil)
https://stackoverflow.com/questions/6064566/concatenate-two-32bit-numbers-to-get-a-64bit-result
```c++
#include <stdint.h>

uint32_t a, b;
uint64_t c;

c = ((uint64_t)a << 32) | b;  
````
## Angle entre deux vecteur
https://openclassrooms.com/forum/sujet/fonction-calcul-d-angle-entre-2-vecteurs-22322  
https://www.mvps.org/directx/articles/math/dot/index.htm  

![text](angle.png)  
arcos(produit scalaire / vector1.lenght*vector2.lenght) = angle en radian = (angle en radian * 180/3.14) degre  

# Error

## error: cannot call constructor 'color_t::color_t' directly
&#x1F34E; error : mColor = colort::color_t(parameter_value);  
&#x1F34F; correct : mColor = color_t(parameter_value);  
expli : il ne faut pas spécifier le namespace de la class/struct. Si on le spécifie, on demande d'utiliser le constructeur "à la main" et aucune allocation n'est faite !  
