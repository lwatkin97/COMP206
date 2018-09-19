
parent=$1

files=`find $parent -name "*.jpg"| xargs ls -rt| xargs basename`

changeName=`echo "$parent"|tr '/' '_'`

outputFile="$changeName.jpg"

convert -append $files $outputFile






###directories=`ls $parent`
#echo $directories
#mkdir newFiles
#list=
#for file in $directories
#do
    #findFile=`ls $parent/$file`
    #list="${list} ${findFile}"
#done

#ls -t {$list}