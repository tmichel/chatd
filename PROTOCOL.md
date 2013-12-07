# chatd protokoll

A kliens és szerver egy saját szöveges protokollon keresztül kommunikál. A
parancsok általános formátuma a következő:

    COMMAND [arguments]

A válasz formátuma a következő:

    CODE message

Sikeres végrehajtás esetén a visszatérési kód:

    100 message

A pontos hibakódok megtalálhatóak a `command.h` header fájlban.

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

### Üzenet küldése a szobába

    TALK room message

Üzenet küldése egy szobába. Üzenet küldés nem sikerül, ha a következők egyike
teljesül:

* a felhasználó nincs a szobában
* nincs ilyen szoba
* nincs a felhasználónak joga beszélni a szobában.
