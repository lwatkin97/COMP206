
parent=$1

files=`ls -lR $parent| sort -k8| grep .jpg| cut -f11 -d " "`

changeName=`echo "$parent"|tr '/' '_'`

outputFile="$changeName.jpg"

convert -append=$files $outputFile






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