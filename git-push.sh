git rm -r --cached app build .cache app compile_commands.json
git remote add origin https://github.com/Minshel/Cile.git
git switch main

git add .
git commit -m "commit"
git push -u origin main --force
