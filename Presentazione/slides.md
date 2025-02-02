---
theme : "cern"
---

# Generazione e risoluzione di labirinti

[Anna Spanò](https://github.com/ottyanna) - 27/03/24

 <p><left><small>licensed under <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">CC BY 4.0</a></small></left></p> 

---

###  Contenuti
- Caratteristiche di un buon labirinto
- Struttura dati
- Generazione: RandomDFS
- Risoluzione: A* search
    - Dimostrazione ammissibilità
    - Ottimalità con coerenza
    - Applicazione a un labirinto
- Riferimenti

---

### Caratteristiche di un buon labirinto
- Ha un bordo ben definito
- Ogni punto deve poter essere raggiungibile
- non è banale

---

# struttura dati

---

### visivamente e ...

rappresentazione "visiva" &rarr; **griglia 2D NxM**

4 movimenti possibili &rarr; &larr; &darr; &uarr;

<img src="images/Griglia.png" width="40%">

<!-- - N=#colonne, M=#righe 
- 4 possibili movimenti &rarr; &larr; &darr; &uarr; -->


---

### ... internamente
struttura interna &rarr; **grafo non orientato come lista di adiacenza "potenziale"**
- vertici = caselle
- archi = passaggi tra le caselle
- vettore di N*M vertici 
- ogni vertice ha 4 adiacenti, a parte i vertici al bordo &rarr; Bordo ben definito dalla connettività &#x2714;

**complessità spaziale:**
- O(N*M) vettore di vertici
- O(N\*M) lista di adiacenza "potenziale" come vettore di vettori (grado di ogni vertice $\leq$ 4)

---

# Generazione : RandomDFS

---

### DFS ...
- si utilizza la capacità del Depth First Search di generare un albero di copertura &rarr; Ogni punto è raggiungibile &#x2714;
- DFS seleziona gli archi "da aprire" &rarr; lista di adiacenza "vera" del labirinto è quella "potenziale" senza muri
- &rarr; Complessità spaziale rimane sempre la stessa per ragioni "visive" 

---

### ... randomizzato

- DFS non randomizzato crea un unico lungo corridoio in base a come è inizializzata la lista (vd sotto)
**RandomDFS:**
- scelgo un vertice adiacente a caso non ancora visitato e lo visito
- appena arrivo a un punto dove non ci sono più mosse disponibili "chiudo" il vertice e faccio il backtracking
- A causa della scelta random è meglio usare uno stack esplicito dove salvare i vertici "in progress"/Grey invece di una chiamata ricorsiva

--

### DFS non randomizzato

lista di adiacenza = &rarr; &larr; &darr; &uarr;

<img src="images/Longcorridor.png" width=50%>

---

## pseudocodice
<pre><code data-line-numbers> RandomDFS(G,start)
    Stack S;
    S.push(start); //O(1)

    while(S non vuoto):
        u = S.top(); //O(1)
        u.col=GREY;
        inizializzare vettore vertici AdjDisponibili/AdjWhite //O(4)=O(1)
        if (AdjWhite non vuoto):
            selezionare vertice random in AdjWhite;
            random.parent=u;
            breakWall(u,random); //O(4)=O(1)
            S.push(random);
        else //nessun movimento possibile
            u.col=BLACK;
            estrarre vertice u da S; //O(1)
</code></pre>

WORST CASE:
- **Complessità spaziale:** O(|V|)=O(N*M) &larr; caso "serpentone"
- **Complessità temporale:**  O(|V|+|E|)=O(N*M) &larr; al massimo 4 adiacenti possibili per la scelta per ogni vertice

---

# Esempio

<img src="images/Dfs 1.png" width=50%>

--

# Esempio

<img src="images/Dfs 2.png" width=50%>

--

# Esempio

<img src="images/Dfs 3.png" width=50%>

---

## same DFS generated maze without and with loops

<img src="images/maze.png" width=40%>

<img src="images/mazeWloops.png" width=41%>

---

# Risoluzione: A* search

---

## A* search
- Algoritmo del 1968
- In realtà è una classe di algoritmi **euristici** che ha come parametro l'euristica scelta
- Lavora su grafi orientati pesati &rarr; labirinto: **tutti i pesi sono 1**
- Come Dijkstra, A* **minimizza** una funzione $\tilde{f}(n)$
- ma **INFORMED SEARCH** vs **UNINFORMED SEARCH** (Dijkstra,BFS,DFS)  
- A* ottimo sotto euristica:
    - **Ammissibile**: $\tilde{h}(n)\leq h(n)$ &rarr; A* trova lo shortest path
    - **Coerente (Consistent):** $h(m,n) + \tilde{h}(m)\leq \tilde{h}(n)$ &rarr; A* esplora meno vertici rispetto ad altri algoritmi ammissibili 

---

## pseudocodice
<pre><code data-line-numbers> A*(G,start,goal) //N.B. necessità di un goal!
    for v in V: //O(N*M)
        v.gTilde=inf;
        v.parent=null;

    start.gTilde=0;
    start.fTilde=h(start,goal);
    
    OPEN={start}; //minHeap
    start.col=GREY;
    
</code></pre>

--

 <pre><code data-line-numbers data-ln-start-from="10"> while(OPEN non vuoto):
        estraggo u da OPEN con fTilde minimo; //log(N)
        if u=GOAL: exit;
        for adj in adjList(u) //con edge=OPEN
            costo = u.gTilde + w(u,adj) //u.gTilde = costo "so far" di u

            // se adj non ancora espanso
            // o il nuovo costo passando per u è minore del precedente 
            if (adj.col=WHITE or costo < adj.gTilde):
                adj.gTilde = costo;
                adj.fTilde = adj.gTilde + h(adj,goal);
                adj.parent = u;
                if adj not in OPEN //adj not GREY, se è BLACK lo riapro
                    adj.col=GREY
                    OPEN.add(adj); //log(N) Binomial heap/O(1) Fibonacci Heap 
                else OPEN.update(adj); //log(N)
        u.col=BLACK; //add to CLOSED list
</code></pre>

**Complessità worst case:**
- **Spaziale**: 
    - O(|V|+|E|)=O(N*M) per adj list
    - O(|V|)=O(N*M) per il MinHeap (in generale $\leq$ |V|)

--

<pre><code data-line-numbers data-ln-start-from="10"> while(OPEN non vuoto):
        estraggo u da OPEN con fTilde minimo; //log(N)
        if u=GOAL: exit;
        for adj in adjList(u) //con edge=OPEN
            costo = u.gTilde + w(u,adj) //u.gTilde = costo "so far" di u
            if (adj.col=WHITE or costo < adj.gTilde):
                adj.gTilde = costo;
                adj.fTilde = adj.gTilde + h(adj,goal);
                adj.parent = u;
                if adj not in OPEN //adj not GREY, se è BLACK lo riapro
                    adj.col=GREY
                    OPEN.add(adj); //log(N) Binomial heap/O(1) Fibonacci Heap 
                else OPEN.update(adj); //log(N)
        u.col=BLACK; //add to CLOSED list
</code></pre>

- **Temporale**:
    - O(|V|) resetMaze
    - O(|V|log|V|) per il pop in OPEN
    - O(|E|log|V|) per il push/update in OPEN considerando che ogni vertice chiuso non viene riaperto con un'euristica coerente &rarr; ogni vertice viene aggiunto una sola volta
    - O(|E|log|V|*f(|V|)) per il calcolo di h(n,m), generalmente O(1)
    - **TOTALE** O((|V|+|E|\*f(|V|))\*log|V|)=O(N\*M\*log(N\*M))
<!--    - **N.B.**: il costo temporale dipende fortemente dalla funzione h(n,m) che non può essere conosciuto a priori --> 

--

**NOTA IMPORTANTE:**
Essendo stato applicato a un labirinto, ossia una struttura t.c. tutti i punti sono raggiungibili tra loro, non è stato necessario aggiungere l'ultimo pezzo del codice, i.e.

```
// Open set is empty but goal was never reached
    return failure
```

---

## Esempio A*

<img src="images/Astar 1.png" width=70%>

--

## Esempio A*

<img src="images/Astar 2.png" width=70%>

--

## Esempio A*

<img src="images/Astar 3.png" width=70%>

--

## Esempio A*

<img src="images/Astar 4.png" width=70%>

---

### Definizione della funzione $f$

<!-- - $t\equiv$ vertice target 

- $s\equiv$ vertice iniziale --> 

<img src="images/Fn.png" width=30%>

- $P^*(m,n)\equiv$ percorso ottimo (di costo minimo) tra m ed n

- $g(n)\equiv$ costo di P*(s,n)

- $h(n)\equiv$ costo di P*(n,t) 

- $f(n)\equiv$ costo totale di P*(s,t) vincolato attraverso n ($f(s)=h(s)$ costo di P*(s,t) non vincolato)

&rarr; **$f(n)=g(n)+h(n)$**

---

### Definizione della funzione di stima $\tilde{f}$

**$f(n)=g(n)+h(n)$ &rarr; stimato $\tilde{f}(n)=\tilde{g}(n)+\tilde{h}(n)$**

- $\tilde{g}(n)\equiv$ stima del costo di P*(s,n), è preso come il costo minore trovato "so far" dall'algoritmo, quindi $\tilde{g}(n)\geq g(n)$

- $\tilde{h}(n)\equiv$ stima del costo di P*(n,t), è **estraibile dal problema** p.e. nel caso in cui si considerasse una mappa, potrebbe essere la distanza in linea d'aria

**&rarr; Si dimostra che se $\tilde{h}(n)\leq h(n)$, ossia se la distanza minima è sottostimata, allora A\* è ammissibile:**

---

## Dimostrazione di ammissibilità di A*

A* è ammissibile se trova il percorso ottimo (quindi di costo minore) da s a t per ogni $\delta$ grafo, i.e. ogni grafo i cui archi abbiano peso maggiore o uguale a $\delta> 0$

---

## Lemma

Per ogni n NON CHIUSO e per ogni percorso ottimo P*(s,n), esiste n' su P* APERTO t.c. $\tilde{g}(n')=g(n')$

## Corollario

Supponendo $\tilde{h}(n) \leq h(n)$  $\forall n $ e supponendo che A* non abbia terminato, allora per ogni percorso ottimo P*(s,t) esiste n' su P* APERTO t.c. $\tilde{f}(n')\leq f(s)$ con $f(s)$ costo reale del percorso ottimo P*(s,t).

<!--
--

### Dimostrazione Lemma

$P*=(s=n_0,n_1,...n_k=n)$

- se s NON APERTO &rarr; n'=s &rarr; $\tilde{g}(s)=g(s)=0$ Q.E.D.
- se s CHIUSO
    
    Sia $\Delta= {{n_i}\text{ su P* CHIUSO t.c. } \tilde{g}(n_i)=g(n_i)}$ , $\Delta \neq \emptyset$ poichè $s\in\Delta$
    
    Sia n* elemento in $\Delta$ con indice più alto
    &rarr; $n*\neq n$ poichè n NON CHIUSO
    Sia n' successore di n* su P* (può essere che n'=n)
    &rarr; $\tilde{g}(n') \leq \tilde{g}(n*) +  w(n*,n')$ per def di $\tilde{g}$
    (n' APERTO e n* CHIUSO &rarr; quindi passata la condizione $\tilde{g}(n*)+w(n*,n')<\tilde{g}(n')$ o $\tilde{g}(n')$ era già minore o si aggiorna all'altro)
    $\tilde{g}(n*)=g(n*)$ poichè $n*\in\Delta$
    e $g(n')=g(n*)+w(n*,n')$ poichè su P*
    &rarr; $\tilde{g}(n')\leq g(n')$ &rarr; ma in gen $\tilde{g}(n')\geq g(n') &rarr; \tilde{g}(n')=g(n')$ con $n'\in OPEN$. Q.E.D.
-->
<!--
--

### Dimostrazione Corollario

per il lemma esiste n' APERTO su P* t.c. $\tilde{g}(n')=g(n')$ &rarr; $\tilde{f}(n')=\tilde{g}(n')+\tilde{h}(n')=g(n')+\tilde{h}(n')\leq g(n')+h(n')=f(n')=f(s)$ con n' su P*
-->

---

## TEOREMA

Se $\tilde{h}(n) \leq h(n)$ $\forall n$, allora A* è ammissibile.

--

### Dimostrazione

P.A. A* termina in t con $\tilde{f}(t)=\tilde{g}(t)>f(s)$.

N.B. $f(s)=h(s)=g(t)$ costo P*(s,t) unconstrained

Per il corollario, prima della terminazione esiste n' APERTO su P* t.c. $\tilde{f}(n')\leq f(s) < \tilde{f}(t)$.
&rarr; n' dovrebbe essere espanso prima di t &rarr; impossibile che A* abbia terminato

---

## Ottimalità di A* sotto euristica coerente

---

## Euristica coerente (consistent)

Per ogni nodo m ed n vale la disuguaglianza triangolare

$h(m,n)+\tilde{h}(m) \geq \tilde{h}(n)$

con h(m,n) distanza lungo P*. 

Considerando la distanza in linea d'aria $d$

$h(m,n) \geq d(m,n)$ &rarr; $h(m,n)+\tilde{h}(m) \geq d(m,n)+\tilde{h}(m) \geq \tilde{h}(n)$

<img src="images/EuConsistent.png" width=40%>

---

**Si può dimostrare che**
**Euristica coerente &rarr; A\* espande meno nodi rispetto a un algoritmo ammissibile A**

**&rarr; $N(A\*,G_{s}) \leq N(A,G_{s})$ dove N numero di nodi espansi.**

Vale $=$ sse A espande gli stessi nodi di A*

Infatti per esempio A* con $\tilde{h}=0$ si riconduce a Dijkstra

Per dimostrarlo si nota che sotto euristica coerente se un nodo n è già stato CHIUSO &rarr; è stato già trovato il P*(s,n) &rarr; $\tilde{g}(n)=g(n)$ &rarr; n non verrà mai riaperto.

---

# A* nei labirinti

---

### Euristica

Come buona euristica è naturale considerare la **Manhattan distance** ($L_1$) in 2D:

$|x_1-x_2|+|y_1-y_2|$

in quanto è la distanza che si avrebbe nel caso in cui non ci fossero muri.

Si dimostra che questa metrica è coerente.

<img src="images/mDist.png" width=30%>

---

<!--# Confronto tra i vari algoritmi

Se si considerano diversi algoritmi, è facile vedere come effettivamente A* nel caso di aggiunta di loops, quindi riomogeneiizando in terreno visiti molti meno nodi a un costo però di Update e add nello heap. La sua efficacia è molto maggiore nel caso di grafi pesati e spazi più grandi senza troppi "ostacoli". Sicuramente visitando Dijkstra tanti nodi quanto BFS, e avendo stessa complessità di A* performa molto peggio.

Complessità effettiva
Considerando un branching medio b
nodo depth d

Att a mettere coppie giuste + mettere vertice colore iniziale giusto!

--->

### A* vs..

<img src="images/AStar.png" width=40%>

### ... BFS/Dijkstra

<img src="images/BFS.png" width=40%>

---

## Riferimenti

<https://web.archive.org/web/20160322055823/http://ai.stanford.edu/~nilsson/OnlinePubs-Nils/PublishedPapers/astar.pdf>

<http://theory.stanford.edu/~amitp/GameProgramming/>


