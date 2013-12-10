# chatd protokoll

A kliens és szerver egy saját szöveges protokollon keresztül kommunikál. A
parancsok általános formátuma a következő:

    COMMAND [arguments]

A válasz formátuma a következő:

    CODE message

Sikeres végrehajtás esetén a visszatérési kód:

    100 message

A pontos hibakódok megtalálhatóak a `const.h` header fájlban.

## Parancsok

### Belépés

    REG username [password]

Regisztráció és autentikáció egyben. Nem létező felhasználó esetén létrehoz
egyet, ha a jelszó is van, akkor állandó felhasználót hoz létre, aki később
is beléphet. Ha nincs jelszó, akkor ideiglenes felhasználó jön létre, ami a
sessionje végén megszűnik.

### Kilépés

    EXIT

Kilép a felhasználó. Minden csatornáról kiléptetjük.

### Jelszó csere/létrehozás

    PWD new [old]

Jelszó csere. Amennyiben a felhasználó csak ideiglenes és nincs még jelszava a
szerveren, akkor regisztrálja a szerver a felhasználót állandó felhasználóként
regisztrálja megadott jelszóval.  Ilyenkor az _old_ paraméter megadása nem
szükséges.

### Csatlakozás szobához

    JOIN room

Amennyiben a szoba még nem létezik, akkor újat hozunk létre. Ha már létezik,
akkor csak belépünk.

### Szoba elhagyása

    LEAVE room

Ha utolsóként hagyjuk el a szobát, akkor az megszűnik. Olyan szobából, aminek
nem vagyunk tagjai, nem tudunk kilépni. Ilyenkor hibaüzenet érkezik válaszként.

### Üzenet küldése a szobába

    TALK room message

Üzenet küldése egy szobába. Üzenet küldés nem sikerül, ha a következők egyike
teljesül:

* a felhasználó nincs a szobában
* nincs ilyen szoba
* nincs a felhasználónak joga beszélni a szobában.

### Privát üzenet

    MSG user message

Privát üzenet küldése egy másik felhasználónak. Ezt csak a másik felhasználó
kapja meg. Ha nincs bejelentkezett felhasználó _user_ névvel, akkor egy
hibakódot küld vissza a szerver.

### Adminisztrátori jog adása

    GRANT room user

Egy szobában egy felhasználónak admin jogot ad. Csak admin jogú felhasználó
adhat ilyen jogot. A művelet meghiúsul, ha a következők egyike teljesül:

* A _room_ szoba nem létezik
* A felhasználónak nincs admin joga.
* A _user_ felhasználó nem tagja a szobának.

### Adminisztrátori parancsok

#### Felhasználó eltávolítása egy szobából

    KICK room user

Ez a parancs csak akkor hajtható végre, ha az adott szobában adminisztrátori
joggal rendelkezünk. Sikeres lefutás esetén a felhasználót időlegesen
eltávolítja a szobából. Sikertelen végrehajtás esetén egy hibakódot kapunk
vissza.

#### Felhasználó elnémítása

Ez egy parancs páros `MUTE` és `VOICE`. Az előbbi elhallgattat egy felhasználót.
Ilyenkor a felhasználó még láthatja, ami a szobában folyik, de nem szólhat
hozzá. Az utóbbi visszaadja egy felhasználó beszéd jogát.

A felhasználók a szobához való csatlakozáskor mindig automatikusan beszédjogot
kapnak.

    MUTE room user
    VOICE room user

Ez a parancs csak akkor hajtható végre, ha az adott szobában adminisztrátori
joggal rendelkezünk.
