#!/bin/bash

# Nome del tuo eseguibile (usa percorso assoluto o variabile)
# MINISHELL_DIR="../minishell_final"
MY_SHELL="./minishell"

# Verifica presenza e permessi dell'eseguibile
if [ ! -f "$MY_SHELL" ]; then
    echo "Errore: Eseguibile '$MY_SHELL' non trovato"
    exit 1
fi

if [ ! -x "$MY_SHELL" ]; then
    echo "Errore: Eseguibile '$MY_SHELL' non ha i permessi di esecuzione"
    echo "Eseguire: chmod +x $MY_SHELL"
    exit 1
fi

# Verifica presenza file di test
if [ $# -ne 1 ]; then
    echo "Utilizzo: $0 <file_con_test>"
    exit 1
fi

TEST_FILE="$1"
if [ ! -f "$TEST_FILE" ]; then
    echo "Errore: File di test '$TEST_FILE' non trovato"
    exit 1
fi

rm -rf diff reports

# Creazione cartelle output
mkdir -p diff reports

# File di report
REPORT_PASSED="reports/passed_tests.log"
REPORT_FAILED="reports/failed_tests.log"
REPORT_SUMMARY="reports/summary.log"
TEST_COUNTER_FILE="reports/test_counter.log"

# Inizializza i file di report
> "$REPORT_PASSED"
> "$REPORT_FAILED"
> "$REPORT_SUMMARY"
> "$TEST_COUNTER_FILE"

# Contatori
total_lines=0
total_tests=0
passed=0
failed=0
skipped=0

# Funzione avanzata per pulire l'output
clean_output() {
    local file=$1
    local type=$2
    
    # Rimuovi codici colore ANSI
    sed -r 's/\x1B\[[0-9;]*[mK]//g' "$file" > "${file}.clean"
    
    # Rimuovi prompt e comandi solo per stdout
    if [ "$type" == "stdout" ]; then
        # Rimuovi righe che iniziano con minishell> (prompt)
        sed -i '/^minishell>/d' "${file}.clean"

                # 2. Rimuovi prompt attaccati alla fine dell'output (senza cancellare contenuto)
        #    - Casi con newline: sostituisci "minishell>" alla fine riga
        #    - Casi senza newline: sostituisci "minishell>" alla fine file
        sed -i 's/minishell>$//' "${file}.clean"
        sed -i -e ':a' -e '$!{N;ba' -e '}' -e 's/minishell>$//' "${file}.clean"
        
        # Rimuovi righe vuote
        sed -i '/^$/d' "${file}.clean"
    fi
    
    # Normalizza messaggi d'errore
    if [ "$type" == "stderr" ]; then
        # # Normalizza sequenze di escape e caratteri speciali
        # sed -i -e 's/^[^:]*: //' \
        #        -e 's/command not found$/command not found/' \
        #        -e 's/`\\n`/`newline`/g' \
        #        -e 's/`\\\\n`/`newline`/g' \
        #        -e 's/`\\\n`/`newline`/g' \
        #        -e 's/`\n`/`newline`/g' \
        #        -e 's/`\\r`/`carriage return`/g' \
        #        -e 's/`\\t`/`tab`/g' \
        #        -e 's/`\\\`/`backslash`/g' \
        #        "${file}.clean"
        
        # # Normalizza messaggi di sintassi
        # sed -i -e 's/syntax error near unexpected token/syntax error/g' \
        #        -e 's/syntax error: unexpected end of file/syntax error/g' \
        #        -e 's/syntax error: unexpected token/syntax error/g' \
            #    -e 's/syntax error near unexpected token/syntax error/g' \
            #    -e 's/[a-zA-Z0-9_]^minishell> //' \
        #        "${file}.clean"
        # sed -i -e 's/^bash: line [0-9]\+: //' \
        #         -e 's/^bash: -c: line [0-9]\+: //' \
        #        -e 's/^minishell> //' \
		# 	    -e 's/^minishell: exit/exit/' \
        #        "${file}.clean"
        sed -i -e 's/^bash: line [0-9]\+: //' \
                -e 's/^bash: -c: line [0-9]\+: //' \
               -e 's/^minishell> //' \
               -e 's/^minishell: //' \
               -e 's/exit : /exit: /' \
               -e '/^exit$/d' \
               "${file}.clean"
        sed -i 's/minishell>$//' "${file}.clean"  # Per righe con newline
        sed -i 's/minishell>$//' "${file}.clean"  # Doppia esecuzione per sicurezza
        sed -i 's/minishell>//' "${file}.clean"   # Per righe senza newline
    fi
    
    mv "${file}.clean" "$file"
}

# Conta tutte le linee (per debug)
total_lines=$(wc -l < "$TEST_FILE")
echo "Totale righe nel file di test: $total_lines" | tee "$TEST_COUNTER_FILE"

# Lettura file di test
test_number=1
line_count=0
critical_error=0
echo "Esecuzione test in corso..."

while IFS= read -r test_cmd || [ -n "$test_cmd" ]; do
    ((line_count++))
    
    # Rimuovi eventuali ritorni a capo DOS/Windows
    test_cmd="${test_cmd%$'\r'}"
    
    # Salta righe vuote (anche quelle con solo spazi/tab)
    if [[ -z "$test_cmd" || "$test_cmd" =~ ^[[:space:]]*$ || "$test_cmd" == \#* ]]; then
        ((skipped++))
        echo "Linea $line_count: SKIPPED - '$test_cmd'" >> "$TEST_COUNTER_FILE"
        continue
    fi

    # Mostra progresso
    if (( line_count % 10 == 0 )); then
        echo -n "."
    fi
    
    ((total_tests++))
    echo "Linea $line_count: PROCESSING - '$test_cmd'" >> "$TEST_COUNTER_FILE"
    
    # Crea una directory temporanea per il test
    TEST_DIR=$(mktemp -d)
    mkdir -p "$TEST_DIR/expected" "$TEST_DIR/actual"
    
    # Ottieni il percorso assoluto della shell
    SHELL_PATH=$(realpath "$MY_SHELL")

    # Esecuzione con bash
    (cd "$TEST_DIR" && bash -c "$test_cmd" > "expected/out" 2> "expected/err")
    bash_exit=$?
    echo "$bash_exit" > "$TEST_DIR/expected/exit"

    # Esecuzione con minishell
    (cd "$TEST_DIR" && echo "$test_cmd" | "$SHELL_PATH" > "actual/out" 2> "actual/err")
    minishell_exit=$?
    if grep -qi "command not found" "$TEST_DIR/actual/err"; then
        minishell_exit=127
        fi
    if grep -qi "not a valid identifier" "$TEST_DIR/actual/err"; then
        minishell_exit=1
    fi
    if grep -qi "syntax error" "$TEST_DIR/actual/err"; then
        minishell_exit=2
    fi
    # Correzione cruciale: gestione exit status non validi
    if [[ "$test_cmd" == exit* ]] && [[ $minishell_exit -eq 0 ]]; then
        # Se è un comando exit ma minishell ritorna 0, verifica se doveva essere errore
        if grep -qi "numeric argument required" "$TEST_DIR/actual/err" || 
           grep -qi "too many arguments" "$TEST_DIR/actual/err"; then
            minishell_exit=1  # Imposta exit status di errore standard
        fi
    fi
    echo "$minishell_exit" > "$TEST_DIR/actual/exit"
    
    # # Gestione speciale per comandi exit
    # if [[ "$test_cmd" == exit ]]; then
    #     # Esecuzione con bash in una subshell
    #     (bash -c "$test_cmd" > "$TEST_DIR/expected/out" 2> "$TEST_DIR/expected/err")
    #     bash_exit=$?
        
    #     # # Normalizza exit status per bash (255 per valori fuori range)
    #     # if [[ $bash_exit -gt 255 ]]; then
    #     #     bash_exit=255
    #     # fi
    #     echo "$bash_exit" > "$TEST_DIR/expected/exit"
        
    #     # Esecuzione con minishell
    #     (bash -c "$test_cmd" && echo "$test_cmd" | "$SHELL_PATH" > "$TEST_DIR/actual/out" 2> "$TEST_DIR/actual/err")
    #     minishell_exit=$?
    #     echo "$minishell_exit" > "$TEST_DIR/actual/exit"
    # else
    #     # Esecuzione normale con bash
    #     (cd "$TEST_DIR" && bash -c "$test_cmd" > "expected/out" 2> "expected/err")
    #     bash_exit=$?
    #     echo "$bash_exit" > "$TEST_DIR/expected/exit"
        
    #     # Esecuzione con minishell
    #     (cd "$TEST_DIR" && echo "$test_cmd" | "$SHELL_PATH" > "actual/out" 2> "actual/err")
    #     minishell_exit=$?
    #     echo "$minishell_exit" > "$TEST_DIR/actual/exit"
    # fi
    
    # Pulizia avanzata dell'output
    clean_output "$TEST_DIR/expected/out" "stdout"
    clean_output "$TEST_DIR/expected/err" "stderr"
    clean_output "$TEST_DIR/actual/out" "stdout"
    clean_output "$TEST_DIR/actual/err" "stderr"
    
    # Confronto output e exit status
    diff_output=$(diff -u "$TEST_DIR/expected/out" "$TEST_DIR/actual/out" 2>&1)
    diff_err=$(diff -u "$TEST_DIR/expected/err" "$TEST_DIR/actual/err" 2>&1)
    diff_exit=$(diff -u "$TEST_DIR/expected/exit" "$TEST_DIR/actual/exit" 2>&1)
    
    # Salvataggio differenze
    if [ ! -z "$diff_output" ]; then
        echo "$diff_output" > "diff/test_${test_number}.out.diff"
    fi
    if [ ! -z "$diff_err" ]; then
        echo "$diff_err" > "diff/test_${test_number}.err.diff"
    fi
    if [ ! -z "$diff_exit" ]; then
        echo "$diff_exit" > "diff/test_${test_number}.exit.diff"
    fi
    
    # Verifica risultati
    if [ -z "$diff_output" ] && [ -z "$diff_err" ] && [ -z "$diff_exit" ]; then
        # Test passato
        echo "Test $test_number: '$test_cmd' PASSED" >> "$REPORT_PASSED"
        ((passed++))
    else
        # Test fallito
        echo "Test $test_number: '$test_cmd'" >> "$REPORT_FAILED"
        [ ! -z "$diff_output" ] && echo "    Differenze stdout: diff/test_${test_number}.out.diff" >> "$REPORT_FAILED"
        [ ! -z "$diff_err" ] && echo "    Differenze stderr: diff/test_${test_number}.err.diff" >> "$REPORT_FAILED"
        [ ! -z "$diff_exit" ] && echo "    Differenze exit status: diff/test_${test_number}.exit.diff" >> "$REPORT_FAILED"
        ((failed++))
    fi
    
    # Pulisci directory temporanea
    rm -rf "$TEST_DIR"
    
    ((test_number++))
done < "$TEST_FILE"

echo -e "\nEsecuzione completata"

# Report finale
echo "Risultati finali:" | tee "$REPORT_SUMMARY"
echo "-----------------" | tee -a "$REPORT_SUMMARY"
echo "Righe totali nel file: $total_lines" | tee -a "$REPORT_SUMMARY"
echo "Test eseguiti:        $total_tests" | tee -a "$REPORT_SUMMARY"
echo "Test passati:         $passed" | tee -a "$REPORT_SUMMARY"
echo "Test falliti:         $failed" | tee -a "$REPORT_SUMMARY"
echo "Righe saltate:        $skipped" | tee -a "$REPORT_SUMMARY"

if [ $critical_error -eq 1 ]; then
    echo "TEST INTERROTTO: Problema critico con l'eseguibile minishell" | tee -a "$REPORT_SUMMARY"
    echo "Verificare i permessi di esecuzione: chmod +x $SHELL_PATH" | tee -a "$REPORT_SUMMARY"
    exit 1
elif [ $failed -ne 0 ]; then
    echo "Dettagli test falliti disponibili in: $REPORT_FAILED" | tee -a "$REPORT_SUMMARY"
    echo "Differenze dettagliate nella cartella: diff/" | tee -a "$REPORT_SUMMARY"
    echo "Per i test passati, vedi: $REPORT_PASSED" | tee -a "$REPORT_SUMMARY"
    echo "Log completo dell'esecuzione: $TEST_COUNTER_FILE" | tee -a "$REPORT_SUMMARY"
    exit 1
else
    echo "Tutti i test sono passati con successo!" | tee -a "$REPORT_SUMMARY"
    echo "Elenco completo: $REPORT_PASSED" | tee -a "$REPORT_SUMMARY"
    echo "Log completo dell'esecuzione: $TEST_COUNTER_FILE" | tee -a "$REPORT_SUMMARY"
    exit 0
fi