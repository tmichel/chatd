# Specifikáció

A házi feladat célja egy chat alkalmazás elkészítése. Ez csak a szervert
foglalja magában, kliens alkalmazást nem. A szerver képes felhasználókat
fogadni. A felhasználók használhatnak eldobható felhasználónevet, vagy
regisztrálhatnak is, amivel megvédhetik az online identitásukat. Miután egy
felhasználó csatlakozott a szerverhez a következő lehetőségek állnak a
rendelkezésére:

* Nyithat új szobát.
* Csatlakozhat már meglévő szobához.
* Beszélhet egy meglévő szobában.
* Kezdeményezhet privát beszélgetést egy másik aktív felhasználóval.
* Amennyiben erre jogosultsága van, moderátori feladatokat is elláthat.
* Lemondhat adminisztrátori jogáról.

Moderátori feladatok:

* Felhasználók időleges eltávolítása egy szobából.
* Felhasználó elhallgattatása. Ilyenkor láthatja mit történik a szobában, de nem szólhat hozzá.
* Adminisztrátori jog adása egy másik felhasználónak.

A szerver képes egyszerre több szobát és privát beszélgetést is kezelni. Minden
szoba nyilvános és bárki csatlakozhat hozzá. A szobát nyitó (tehát az elsőként
csatlakozó) felhasználó automatikusan adminisztrátorrá válik. Nincs lehetőség
szobák regisztrálására, így miután az utolsó felhasználó is elhagyta a szobát,
az megszűnik.

A szerver a nyilvános beszélgetéseket naplózza.