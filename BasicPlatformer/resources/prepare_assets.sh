
#Copy assets to new folder
echo \*\* Copying assets folder to temporary folder...
cp -r ../assets/ assets_temp/

#Remove all .png extensions
echo \*\* Removing .png extensions...
find ./assets_temp/ -type f -name '*.png' | while read f;
	do mv "$f" "${f%.png}";
done

#Remove all .ogg extensions
echo \*\* Removing .ogg extensions...
find ./assets_temp/ -type f -name '*.ogg' | while read f;
        do mv "$f" "${f%.ogg}";
done

# Remove all .tsx extensions
echo \*\* Removing .tsx extensions...
find ./assets_temp/ -type f -name '*.tsx' | while read f;
        do mv "$f" "${f%.tsx}";
done

# Remove all .tmx extensions
echo \*\* Removing .tmx extensions...
find ./assets_temp/ -type f -name '*.tmx' | while read f;
        do mv "$f" "${f%.tmx}";
done

# Remove all Tiled project files
echo \*\* Deleting Tiled project files...
find ./assets_temp/ -type f -name '*.tiled-project' | while read f;
        do rm -f "$f"
done
find ./assets_temp/ -type f -name '*.tiled-session' | while read f;
        do rm -f "$f"
done

#Create compressed archive
echo \*\* Creating compressed folder...
7z a data.zip ./assets_temp/* > /dev/null
echo \*\* Moving zipped data dir to folder...
mv data.zip ../data.dat

#Remove temporary dir
echo \*\* Removing temporary folder...
rm -rf assets_temp/
