python -m PyInstaller --onefile --add-data "assets;assets" --noconsole src/main.py
move "dist\main.exe" "dist\timingtool.exe"
