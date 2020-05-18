# Class Vector

## Nepilnavertė alternatyva std::vector konteineriui
### Naudota literatūra:
- Skaidrės
- [cppreference.com](https://en.cppreference.com/w/cpp/container/vector)
- [C++ primer](https://www.amazon.com/Primer-5th-Stanley-B-Lippman/dp/0321714113)
  
## Šio Vector konteinerio struktūra aprašyta pagal [cppreference.com](https://en.cppreference.com/w/cpp/container/vector)

## 5 pavyzdinės Vector konteinerio funkcijos:

## Efektyvumo/spartos analizė: **std::vector vs Vector** naudojant push_back()

| Dydis    | std::vector | Vector  |
|----------|-------------|-------------|
| 10 000     | 0.0003163  | 0.0004887   |
| 100 000   | 0.0033556 | 0.0028523 |
| 1 000 000  | 0.0270391 | 0.0251484 |
| 10 000 000 | 0.283518  | 0.28707 |
| 100 000 000 | 2.66271  | 2.69369 |

## Perskirstymo analizė: **std::vector vs Vector** testuojant su 100000000 studentų

Perskirstymai
* std::vector - 26 kartai
* Vector - 26 kartai

##  Efektyvumo/spartos analizė: **std::vector vs Vector** testuojant su [v2.0](https://github.com/Definitelynotaspruce/2-oji_uzduotis)

**10 000000 studentu**
* std::vector - 30.9
* Vector - 25.58
