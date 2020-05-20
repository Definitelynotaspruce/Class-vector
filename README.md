# Class Vector

## Nepilnavertė alternatyva std::vector konteineriui
### Naudota literatūra:
- Skaidrės
- [cppreference.com](https://en.cppreference.com/w/cpp/container/vector)
- [C++ primer](https://www.amazon.com/Primer-5th-Stanley-B-Lippman/dp/0321714113)
- [NeonRice](https://github.com/NeonRice)
  
## Šio Vector konteinerio struktūra aprašyta pagal [cppreference.com](https://en.cppreference.com/w/cpp/container/vector)

## Kelios pavyzdinės Vector konteinerio funkcijos:

### erase()
Funkcija, kuriai paduodamas iteratorius *pos* (position).
*alloc.destroy* sunaikina *pos* elementą, *uninitialized_move* paslenka buvusius elementus per vieną vienetą atgal.
```shell
template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator pos)
{
    iterator it = &first[pos - first];
    alloc.destroy(it);
    std::uninitialized_move(it + 1, avail--, it);
        
    return it;
}
```
### insert()
Funkcija, kuriai paduodamas iteratorius *pos* (position) bei *val* (reikšmė). *uninitialized_move* paslenka elementus per vieną vienetą priekin, *val* priskiriamas savo pozicijai.
```shell
template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, const T &val)
{
    auto new_pos = pos - first;
    if (avail == limit)
        grow();

    iterator it = &first[new_pos];
    std::uninitialized_move(it, avail++, it + 1);
    *(it) = val;

    return it;
}
```
### create()
Funkcija, reikalinga Vektoriaus konstravimui iš elementų eilutės. Jai paduodamas [initializer_list](https://en.cppreference.com/w/cpp/utility/initializer_list). *alloc.allocate* išskiria vietos pagal *list.size()*, tuomet elementai po vieną užpildomi su *unchecked_append*.
```shell
template <typename T>
void Vector<T>::create(std::initializer_list<T> list)
{
    first = avail = alloc.allocate(list.size());
    limit = first + list.size();
    for (auto &val : list)
        unchecked_append(val);
}
```

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
