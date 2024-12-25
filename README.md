#### Copyright - Andrei-Cristian IVAN, 311CAa 2024-2025

# Image editor

Imaginea este stocată într-o structură care memorează dimensiunile ei, valoarea maximă admisă pentru un pixel, tipul imaginii și valorile pixelilor. Imaginile suportate sunt cele în nuanțe de gri (grayscale) și cele color, pentru care se pot ține minte valorile pixelilor pe 3 canale de culori diferite (RGB). În selecție, vor fi reținute doar colțurile stânga-sus și dreapta-jos.

În `image_editor.c` vom parsa fiecare rând din `stdin` și vom extrage primul cuvânt de pe fiecare rând. În funcție de primul cuvânt, vom selecta comanda aferentă. Toate implementările comenzilor le vom găsi în `queries.c`.

## LOAD

Mai întâi, se verifică dacă există o imagine precedentă alocată. În caz afirmativ, ea este dealocată. Apoi, parsăm calea relativă a fișierului dorit pentru încărcare și se verifică dacă există acel fișier. Dacă există fișierul dorit, îl vom încărca în memorie. Toate funcțiile care se ocupa cu procesarea imaginii (citiri, afișări etc.) se găsesc în `image.c`. La fiecare încărcare de imagine, vom presupune că selecția curentă cuprinde toată imaginea.

Pentru funcțiile următoare, mereu se va verifica dacă există sau nu o imagine încărcată.

## SELECT

În această funcție vom actualiza selecția curentă. Mai întâi, vom testa dacă se dorește să selectăm toată imaginea. Vom încerca să vedem dacă prin parsarea următorului cuvânt obținem `ALL`. În caz contrar, vom încerca să parsăm $4$ numere, $x_1, y_1, x_2, y_2$, care reprezintă colțurile stânga-sus și dreapta-jos ale selecției. Vom proba dacă există toate cele $4$ numere și dacă acestea sunt cuprinse în matrice. În caz afirmativ, actualizăm selecția și ne vom asigura că $x_1 < x_2$ și $y_1 < y_2$. Avem atenție să afișăm eroare în cazul în care $x_1 = x_2$ sau $y_1 = y_2$, pentru că selecțiile noastre sunt de forma $[A, B)$, deci o selecție de forma $[A, A)$ ar fi echivalent cu mulțimea vidă.


## CROP

În această funcție vom reduce imaginea la selecția curentă. Vom inițializa o copie cu dimensiunile date de selecție, în care vom copia pixelii din imaginea originală. Vom șterge imaginea originală și vom copia imaginea auxiliară în ea. Avem grijă să actualizăm și selecția la imaginea curentă.

## SAVE

Vom citi calea relativă a fișierului în care vom salva imaginea curentă. Vom mai încerca să deducem apoi dacă se dorește afișarea imaginii în format ASCII sau binar. În funcție de tipul imaginii, imaginea va fi salvată în formatul dorit.

## APPLY

Aplicarea unui filtru _(nucleu)_ se poate face numai pe imagini color, deci vom verifica mai întâi că imaginea nu este de tip grayscale. Apoi, vom parsa opțiunea dorită și ne vom asigura că este una dintre cele $4$ tipuri de opțiuni suportate. Vom aplica nucleul pe selecția curentă. Înainte de aplicarea nucleului, avem grijă ca selecția să nu cuprindă conturul, întrucât nucleul se poate aplica doar pe celule care au toți vecinii în matrice. Vom inițializa o copie a imaginii, în care vom salva pixelii modificați (pe care îi validăm cu _clamp_), iar apoi vom șterge imaginea originală pentru a o salva pe cea nouă.

## ROTATE

Rotațiile suportate sunt doar cele care sunt multiplu de $90\degree$. De asemenea, trebuie să avem în vedere că rotațiile sunt congruente modulo $360\degree$. Deci va avea sens să luam restul unghiului la împărțirea cu $360$. Numărul de rotații la $90\degree$ e unghiul împărțit la $90$. Dacă numărul de rotații este negativ, vom face $4 - rotatii$ de $90\degree$. Rotim imaginea de câte ori este necesar și actualizăm imaginea nouă. Avem grijă ca selecția fie să cuprindă toata imaginea, fie să fie un pătrat. În primul caz, avem grijă dacă rotim imaginea la $90\degree$ sau la $270\degree$ să inversăm dimensiunile și selecția imaginii.

## HISTOGRAM

Histograma se poate realiza doar pe imagini grayscale, deci ne vom asigura că excludem imaginile de tip color. Apoi vom încerca să parsăm numărul maxim de stele și numărul de binuri. Ne asigurăm că numărul de binuri este putere de $2$ și că e diferit de $0$. Vom începe apoi să calculăm frecvențele și să aflăm frecvența maximă. Apoi, putem afișa histograma.

## EQUALIZE

Ca la histogramă, egalizarea o putem face doar pe imagini grayscale, deci ne vom asigura că excludem imaginile color. Vom începe să calculăm frecvențele pentru fiecare valoare de pixel posibilă. Apoi, pentru a optimiza calculele, pentru fiecare valoare, în loc să calculăm de fiecare dată $\displaystyle \sum_{i = 0}^{img[i][j]} fr[i]$, vom calcula o sumă parțială pe prefix pe frecvențe $h$, unde $h[i] = h[i - 1] + fr[i]$. Aplicăm apoi formula de egalizare și vom valida pixelii cu _clamp_.

## EXIT

Vom dealoca imaginea curentă și vom închide programul.