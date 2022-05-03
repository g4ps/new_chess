sed 's/{[^}]*}//g' | sed 's/\[[^\]*\]//g' | sed 's/([^)]*)//g'  | sed '/^[^1]/d' | sed '/^[[:space:]]*$/d'
