#!/bin/bash

# Verifică dacă există argumente
if [ "$#" -eq 0 ]; then
    echo "Utilizare: $0 fisier1 fisier2 ..."
    exit 1
fi

while true; do
    echo "\nMeniu:"
    echo "1. Afiseaza tipul fiecarui fisier"
    echo "2. Verifica permisiunile fisierului"
    echo "3. Afiseaza numarul de linii pentru fisierele text"
    echo "4. Creeaza un back-up al fisierelor"
    echo "5. Sorteaza alfabetic continutul fisierelor text"
    echo "6. Iesire"
    read -p "Alegeti o optiune: " optiune

    case $optiune in
        1)
            for fisier in "$@"; do
                if [ -e "$fisier" ]; then
                    file "$fisier"
                else
                    echo "Fisierul $fisier nu exista."
                fi
            done
            ;;
        2)
            for fisier in "$@"; do
                if [ -e "$fisier" ]; then
                    ls -l "$fisier"
                else
                    echo "Fisierul $fisier nu exista."
                fi
            done
            ;;
        3)
            for fisier in "$@"; do
                if [ -f "$fisier" ] && file "$fisier" | grep -q "text"; then
                    echo "$fisier: $(wc -l < "$fisier") linii"
                else
                    echo "$fisier nu este un fisier text."
                fi
            done
            ;;
        4)
            for fisier in "$@"; do
                if [ -e "$fisier" ]; then
                    cp "$fisier" "$fisier.bak"
                    echo "Backup creat pentru $fisier: $fisier.bak"
                else
                    echo "Fisierul $fisier nu exista."
                fi
            done
            ;;
        5)
            for fisier in "$@"; do
                if [ -f "$fisier" ] && file "$fisier" | grep -q "text"; then
                    sort "$fisier" -o "$fisier"
                    echo "Fisierul $fisier a fost sortat."
                else
                    echo "$fisier nu este un fisier text."
                fi
            done
            ;;
        6)
            echo "Iesire..."
            exit 0
            ;;
        *)
            echo "Optiune invalida!"
            ;;
    esac

done
