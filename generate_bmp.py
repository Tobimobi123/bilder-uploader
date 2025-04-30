from PIL import Image, ImageDraw

# Bildgröße
width = 800
height = 480

# Bild erzeugen (RGB-Modus für 24-bit BMP)
image = Image.new('RGB', (width, height), 'white')
draw = ImageDraw.Draw(image)

# Schwarze Streifen als Testmuster
for y in range(0, height, 20):
    draw.rectangle([0, y, width, y + 10], fill='black')

# Speichern als unkomprimiertes 24-Bit BMP
image.save("latest.bmp", format="BMP")

print("✅ BMP-Datei 'latest.bmp' wurde erstellt.")
