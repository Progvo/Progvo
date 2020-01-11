# Progvo-PM 0.9-skizo

Progvo-PM estas baza programada medio por la programlingvo Progvo. Ĝi inkluzivas Ekio redaktilon kaj Progvo interpretilon.

Ĝi estas skribita en C++ kaj uzas SFML por la grafika fasado.

![Saluton, Mondo!](https://progvo.dev/Bildoj/Progvo-PM_Saluton_Mondo.png)

## Direktivoj de programtradukado

### Sub Debian/Ubuntu

Oni povas kompili Progvo-PM kun g++ kaj make. Akiru ankaŭ [SFML](https://www.sfml-dev.org/), uzata por la grafika fasado. Oni devus instali Git por akiri la fontkodon.

Sub Debian aŭ Ubuntu, oni povas simple instali ilin kiel ĉefuzanto, kun:

```bash
apt install g++ make libsfml-dev git
```

Poste, elŝutu la fontdosierojn, `cd` al la Progvo-PM dosierujo, kaj uzi `make`:

```bash
git clone https://github.com/Pttn/Progvo.git
cd Progvo/Progvo-PM
make
```

### Sub Windows x64

Netestita.

Oni povas kompili Progvo-PM sub Windows, ekzemple kun MSYS2. Unue, instalu [MSYS2](http://www.msys2.org/) (vidu la direktivojn en la retejo), poste malfermu la MSYS **MinGW-w64** konzolon, kaj instalu la ilojn kaj dependecojn:

```bash
pacman -S make git
pacman -S mingw64/mingw-w64-x86_64-sfml
```

Klonu la Progvo deponejon kun `git`, `cd` al la Progvo-PM dosierujo kaj kompilu kun `make`.

## Uzo

Esploru dosierojn kun la dekstra panelo, skribu kodon en la teksta kampo.

Elektu dosieron en la esplorilo kaj alklaku je "Ŝargi" por ŝargi ĝian datumojn. Por konservi la tekston, skribu ĝia nomo kaj alklaku je "Konservi".

Interpretu vian kodon alklakanta je "Plenumi". Eraroj estas vidigota en la Progvo Konsolo.

Ekzemploj de Progvo fontkodoj estas disponebla.

### Specialaj signoj

Por la kromsignaj Esperantaj literoj, uzu ekzemple <kbd>Alt</kbd> + <kbd>C</kbd> por `ĉ`. Aliaj specialaj signoj:

* <kbd>Alt</kbd> + <kbd>!</kbd> → `¬`
* <kbd>&</kbd> → `∧`
* <kbd>Alt</kbd> + <kbd>&</kbd> → `∨`
* <kbd>Alt</kbd> + <kbd>*</kbd> → `×`
* <kbd>Alt</kbd> + <kbd>/</kbd> → `÷`
* <kbd>Alt</kbd> + <kbd>v</kbd> → `↓`
* <kbd>Alt</kbd> + <kbd>^</kbd> → `↑`
* <kbd>Alt</kbd> + <kbd><</kbd> → `←`
* <kbd>Alt</kbd> + <kbd>></kbd> → `→`
* <kbd>$</kbd> → `★`
* <kbd>Alt</kbd> + <kbd>$</kbd> → `§`

Vidi ankaŭ la [Ekio difinon](https://progvo.dev/Ekio.html).