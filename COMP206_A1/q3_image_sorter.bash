
directory=$1

files=`find $directory -name "*.jpg"| xargs ls -rt`

changeName=`echo "$directory"|tr '/' '_'`

outputFile="$changeName.jpg"

convert -append $files $outputFile

