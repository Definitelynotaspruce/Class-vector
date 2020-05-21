# Class Vector

## Beveik pilnavertė alternatyva std::vector konteineriui
### Naudota literatūra:
- Skaidrės
- [cppreference.com](https://en.cppreference.com/w/cpp/container/vector)
- [C++ Primer (5th Edition)](https://www.amazon.com/Primer-5th-Stanley-B-Lippman/dp/0321714113)
  
### Šio Vector konteinerio struktūra aprašyta pagal [cppreference.com](https://en.cppreference.com/w/cpp/container/vector)

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

### erase()
Funkcijai paduodami du iteratoriai. Elementai nuo *start* iki *last* yra ištrinami su *alloc.destroy*, tuomet *uninitialized_move* paslenka elementus į atitinkamą vietą.
```shell
template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator start, const_iterator last)
{

    iterator it_start = &first[start - first];
    iterator it_end = &first[last - first];
    auto diff = it_end - it_start;

    for (auto it = it_start; it != it_end; ++it)
        alloc.destroy(it);

    std::uninitialized_move(it_end, avail, it_start);
    avail = avail - diff;

    return it_end;
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
### shrink_to_fit()
Funkcija, kuri capacity() sumažina iki size() dydžio. *alloc.allocate* išskiria tik *size()* dydžio vietą, *uninitialized_copy* nukopijuoja buvusius elementus. *alloc.deallocate* atlaisvina prieš tai naudotą vietą.
```shell
template <typename T>
void Vector<T>::shrink_to_fit()
{    
    auto temp_size = size();
    iterator temp_first = alloc.allocate(size());
    std::uninitialized_copy(first, first + size(), temp_first);
    alloc.deallocate(first, capacity());

    first = temp_first;
    limit = avail = first + temp_size;
}
```
### create()
Funkcija, reikalinga Vektoriaus konstravimui iš elementų eilutės. Jai paduodamas [initializer_list](https://en.cppreference.com/w/cpp/utility/initializer_list). *alloc.allocate* išskiria vietos pagal *list.size()*, tuomet elementai po vieną užpildomi su *alloc.construct*.
```shell
template <typename T>
void Vector<T>::create(std::initializer_list<T> list)
{
    first = avail = alloc.allocate(list.size());
    limit = first + list.size();
    for (auto &val : list)
       alloc.construct(avail++, val);
}
```
## Testavimai

### Kompiuterio specifikacijos:
| CPU                           | RAM | SSD    |
| ----------------------------- | --- | ------ |
| Intel Core i5 7th Gen 2.5 GHz | 8GB | 256 GB |

## Efektyvumo/spartos analizė: **std::vector vs Vector** naudojant push_back()

Analizė buvo atliekama ir su optimizavimo flag'u *-O3* ir be. 
*std::vector* buvo nežymiai pranašesnis už *Vector* su 10 000 - 100 000 000,  tačiau testuojant su 1000 000 000 (milijardu) laiko skirtumas be optimizacijos flago buvo dvigubas. 

| 1000 000 000 | -O3     | (be optimizacijos) |
| ------------ | ------- | ------------------ |
| std::vector  | 15.9032 | 24.0411            |
| Vector       | 23.6825 | **41.951**         |

Visi rezultatai:
| Dydis        | std::vector -O3 | std::vector | Vector -O3 | Vector    |
| ------------ | --------------- | ----------- | ---------- | --------- |
| 10 000       | 0.000494        | 0.000366    | 0.0002259  | 0.0004167 |
| 100 000      | 0.0018134       | 0.002944    | 0.0020645  | 0.0030264 |
| 1 000 000    | 0.0136494       | 0.0251377   | 0.014204   | 0.0266352 |
| 10 000 000   | 0.157973        | 0.259689    | 0.183034   | 0.293446  |
| 100 000 000  | 1.49883         | 2.53095     | 1.68619    | 2.75768   |
| 1000 000 000 | 15.9032         | 24.0411     | 23.6825    | 41.951    |

### Išvados:
- *Vector* yra lėtesnis visais atvejais
- *Vector* nėra tinkamai optimizuotas
- Norint naudotis *Vector* patartina naudoti ir optimizacijos flag'us

## Perskirstymo analizė: **std::vector vs Vector** testuojant su 10000000 studentų

Perskirstymai
* std::vector - 25 kartai
* Vector - 25 kartai

### Išvados:
- Atminties perskirstymas *Vector* vyksta lygiai tiek pat kartų kiek ir *std::vector*

##  Efektyvumo/spartos analizė: **std::vector vs Vector** testuojant su [v2.0](https://github.com/Definitelynotaspruce/2-oji_uzduotis)

Testuojant buvo naudojamas *-O3* flag'as.
**10 000 000 studentų perrašymas (split'inimas)**
* std::vector -  2.89s
* Vector -  4.04s

### Išvados:
- *Vector* gali būti pilnavertė alternatyva programai [v2.0](https://github.com/Definitelynotaspruce/2-oji_uzduotis)
- *Vector* sėkmingai dirba su klase *Studentas*
- *Vector* yra lėtesnis už *std::vector*

## Norint pratestuoti push_back() funkciją su *Vector* ir *std::vector*
1. Nusiklonuoti arba atsisiųsti repozitoriją
2. Atsisiusti ir isidiegti C++ kompiliatorių (pvz GNU)
3. Atsidaryti terminalą toje atsisiųsto failo vietoje ir įrašyti komandą *make* 
4. Rašyti *./main* arba *main.exe* (jei Windows)