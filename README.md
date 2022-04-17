# RacingSimulator2D
[![All Contributors](https://img.shields.io/badge/all_contributors-4-orange.svg?style=flat-square)](https://github.com/Bb4k/RacingSimulator2D/blob/main/README.md#contributors)
[![license](https://img.shields.io/github/license/DAVFoundation/captain-n3m0.svg?style=flat-square)](https://github.com/Bb4k/RacingSimulator2D/blob/main/LICENSE)

Am dezvoltat un joc de tip arcarde, in care scopul este sa incerci sa scapi de o masina de politie. Punctele se obtin in functie de cate masini care vin din contrasens poti depasi. Viteza creste, iar masinille par ca se inmultesc, Daca incetinesti prea mult, pierzi. FUGI!

## Cerinte

### 8p: aspecte "punctuale"

- [x] utilizarea primitivelor;
- [x] utilizarea transformarilor (indicate cu functii standard sau folosind matrice);
- [x] compunerea transformarilor; stive de matrice;
- [x] input interactiv (utilizare mouse, citire date din fisier/de la consola)

### 4p:

- [x] originalitate, caracter inovativ, grad de elaborare

### 3p:

- [x] documentatia: continut, claritatea descrierii proiectului si a solutiilor propuse, evidentierea unor aspecte punctuale din lista de mai sus (primitive, transformari, compunerea transformarilor), argumentarea originalitatii, redactarea textului;
- [ ] prezentarea proiectului la laborator.


## Aspecte punctuale:
1. TRANSFORMARI:<br />
&emsp;Deplasarea pe Ox si/sau Oy al obiectului principal si ale a celor secundare
2. TRANSFORMARI COMPUSE:<br />
&emsp;Rotatia si translatia obiectului principal
3. STIVE DE MATRICE / PRIMITIVE:<br />
&emsp;Compunerea primitivelor in realizarea texturilor grafice
4. INPUT INTERACTIV:<br />
&emsp;Input de la tastatura pentru actiunile jocului si introducerea de caractere in scopul memorarii jucatorului. input de la mouse pentru apasarea butoanelor si actualizarea dinamica a meniulu principa
5. EFECTE SONORE:<br />
&emsp;Diverse efecte sonore in momente cheie ale aplicatiei, coloana sonora de inceput/final, diverse efecte.

## Originalitate:
&emsp;Proiectul se bazeaza pe un workflow alcatuit dintr-un sistem de ecrane, asa cum se poate vedea si in poza de mai jos. Sistemul are la baza constante predefinite pentru toate starile jocului (MAIN_MENU, IN_GAME etc.). Jocul prezinta doua moduri de joc, FREERUN si CAMPAIGN (in dezvoltare) si ambele au ca scop evitarea cat mai multor masini si scaparea de masina de politie.<br />
&emsp;Originalitatea din meniul principal este realizata de miscarea in “slow motion” in momentul cand cursorul sta nemiscat si miscarea in “fast forward” cand miscam cursorul, dar si prin prezenta meniului de optiuni, de unde putem micsora sau marii volumul melodiilor de fundal.<br />
&emsp;Alta nota de originalitate provine din ecranul de configurarea al jocului de unde ne putem alege modelul masinii, culoarea masinii (culoarea aleasa ultima data o sa fie prezenta in toate fundalurile), modul de joc si username-ul.<br />
&emsp;Urmatoarele note de originalitate sunt redate de prezenta unui “Easter Egg” in-game (in momentul inceperii jocului, masina jucatorului trebuie controlata astfel incat sa ajunga cat mai in stanga ecranului si sa nu mai fie vizibila, iar dupa putin timp se va transforma intr-o masina de politie) si de prezenta unui ecran de leaderboard la finalul jocului, unde sunt precizate tot 5 scoruri si numele jucatorilor respectivi. Aceste nume sunt salvate intr-un fiser extern, fapt ce permite retinerea scorurilor si cand nu ruleaza jocul.

## Workflow (screens)

![SCREENS WORKFLOW](https://user-images.githubusercontent.com/30263894/162589090-fd02868e-5f7a-40d0-a9e5-c08de79e682f.png)

## Contributors
<table>
  <tr>
    <td align="center"><a href="https://github.com/VertigoM"><img src="https://avatars.githubusercontent.com/u/63092692?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Mihai M. Alecu</b></sub></a></td>
    <td align="center"><a href="https://github.com/DragosBalmau"><img src="https://avatars.githubusercontent.com/u/30263894?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Dragoș C. Bălmău</b></sub></a></td>
    <td align="center"><a href="https://github.com/TIPYexe"><img src="https://avatars.githubusercontent.com/u/53595545?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Robert A. Nicolescu</b></sub></a></td>
    <td align="center"><a href="https://github.com/VictorAndreiCotescu"><img src="https://avatars.githubusercontent.com/u/63092892?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Victor A. Cotescu</b></sub></a></td>
  </tr>
</table>
