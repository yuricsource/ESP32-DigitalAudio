mkdir temp;
mkdir temp2;
mkdir out;
for file in *;
do 
mpg321 -w ./temp/${file}.wav ${file};
sox ./temp/${file}.wav -r 16000 ./temp2/${file}.wav;
sox ./temp2/${file}.wav ./out/${file}.wav remix 1;
done;
rm -r temp;
rm -r temp2;
