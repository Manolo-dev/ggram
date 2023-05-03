# features

## Verrouillage de branche

`!!` $\to$ verrouille la branche
```
<program> ::= <def> | <call> ;

<def> ::= "def" <name> !! "(" { <arg> } ")" ;
```

Si `"def"` et `<name>` sont vérifiés, alors on verrouille la branche et on vérifie `"(" { <arg> } ")"`. Si la vérification de la suite de la branche échoue, on empèche `<program>` de vérifier d'autres branches.

`??` $\to$ déverrouille la branche. Le vérouillage est annulé. Le dévérouillage est optionnel.
```
<def> ::= <definition> | <proto> ;

<definition> ::= "def" <name> !! "(" [ { <arg> } ] ")" ?? "{" { <expr> } "}" ;

<proto> ::= "def" <name> !! "(" [ { <arg> } ] ")" ";" ;
```

## Appel et définition de fonction

Appel d'une fonction "foo"

```
<expr> ::= <a> <b> <c> :foo <d>
```

Définition d'une fonction "foo"

```c++
bool foo(IT & current, IT end) {
    ...
}
```

## Vérification d'unicité

```
<html-balise> ::= "<" <html-name> $name ">" <html-content> "</" <$name> ">" ;
```

## Listes et scopes

Dans le lexer :

```
]liste "a" "b" "c"
```

Dans le parser :

`<nomListe>` Vérifie que que le Token est bien dans la liste `nomListe`

`]+nomListe` Ajoute le dernier Token à la liste `nomListe`

`]-nomListe` Retire le dernier Token à la liste `nomListe`

`]>nomListe` Augmente la scope de la liste `nomListe`

`]<nomListe` Réduit la scope de la liste `nomListe`

`]!nomListe` Vide la liste `nomListe`

`]nomListe "a" "b" "c" [` Remplit la liste `nomListe` avec les Tokens `"a"`, `"b"` et `"c"`


```
<expr> ::= "{" ]>variable <expr> "}" ]<variable
         | "var" <id> ]+variable [ "=" <expr> ]
         | <variable>
         ;
```

## Répétitions contraintes

```
<expr> ::= { <a> }[2:5] ;
```

## Préprocesseur

Dans le lexer :

```
#MIN_A 10
#MAX_A 100
```

Dans le parser :

```
<expr> ::= { <a> }[#MIN_A:#MAX_A] ;
```

## Erreur multiples

Dans le lexer :

```
.resume ";"
```

Dans le parser :

```
<expr> ::= "{" >> "}" << ;
```