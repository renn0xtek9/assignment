#!/bin/bash
SOURCE_FOLDER="$1"
DESTINATION_FOLDER="$2"
echo "" > "$DESTINATION_FOLDER/build_tree.html"
while read -r filepath
do
    #shellcheck disable=SC2001,SC2046,SC2005,SC2086
    target_name=$(echo $(basename $filepath)|sed 's/cmake\.dot\.//g')
    echo "<h2>Target: $target_name</h2>" >> "$DESTINATION_FOLDER/build_tree.html"
    # shellcheck disable=SC2086,SC2046
    echo "<img src="$(basename $filepath).png" alt="$(basename $filepath).png">" >> "$DESTINATION_FOLDER/build_tree.html"
    dot -T png "$filepath" -o "$DESTINATION_FOLDER"/"$(basename "$filepath")".png
done < <(find "$SOURCE_FOLDER" -name "*.dot*" |sort )
