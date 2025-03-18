import os
import shutil
from PIL import Image  # pip install pillow
import math

# Définition des dossiers
DOSSIER_INPUT = "input"
DOSSIER_OUTPUT = "output"
DOSSIER_SRC = os.path.join(DOSSIER_OUTPUT, "src")
OUTPUT_FILE = os.path.join(DOSSIER_OUTPUT, "output.txt")
MAX_IMAGES = 64

def nettoyer_output():
    """Supprime tout le contenu du dossier output."""
    if os.path.exists(DOSSIER_OUTPUT):
        shutil.rmtree(DOSSIER_OUTPUT)  # Supprime tout le dossier output
    os.makedirs(DOSSIER_SRC, exist_ok=True)  # Recrée output/src

def trouver_images(dossier):
    """Trouve toutes les images dans le dossier, triées par nom."""
    images = sorted(
        [f for f in os.listdir(dossier) if f.lower().endswith((".png", ".jpg", ".jpeg", ".bmp", ".gif"))]
    )
    return images[:MAX_IMAGES]  # Limite à MAX_IMAGES images

def couleur_proche(couleur, palette):
    """Retourne l'index de la couleur la plus proche dans la palette."""
    return min(range(len(palette)), key=lambda i: math.sqrt(sum((couleur[j] - palette[i][j])**2 for j in range(3))))

def demander_dimensions(nom_image, largeur_actuelle, hauteur_actuelle):
    """Demande à l'utilisateur la taille souhaitée pour cette image."""
    print(f"\n📏 Image : {nom_image} ({largeur_actuelle}x{hauteur_actuelle})")
    while True:
        try:
            largeur = int(input("Entrez la NOUVELLE largeur : "))
            hauteur = int(input("Entrez la NOUVELLE hauteur : "))
            if largeur > 0 and hauteur > 0:
                return largeur, hauteur
            else:
                print("❌ Les valeurs doivent être positives.")
        except ValueError:
            print("❌ Veuillez entrer des nombres valides.")

def image_en_texte(image_path, largeur_cible, hauteur_cible):
    """Convertit une image en texte basé sur la palette."""
    lett = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    palette_fixe = [
        [63, 67, 40], [95, 113, 50], [148, 173, 57], [194, 214, 79], [239, 243, 124],
        [227, 230, 172], [165, 198, 124], [115, 154, 112], [77, 102, 89], [52, 63, 65],
        [40, 46, 59], [26, 31, 46], [30, 49, 75], [47, 76, 108], [61, 128, 163],
        [99, 196, 204], [154, 229, 213], [229, 239, 239], [186, 201, 205], [141, 153, 164],
        [105, 111, 128], [65, 68, 83], [184, 161, 194], [126, 101, 155], [92, 58, 111],
        [57, 39, 94], [47, 25, 62], [78, 26, 73], [123, 35, 76], [178, 54, 87],
        [209, 105, 116], [237, 170, 163], [238, 203, 144], [225, 168, 69], [197, 120, 53],
        [141, 72, 48], [228, 114, 89], [195, 60, 64], [141, 54, 73], [92, 43, 52],
        [60, 37, 43], [104, 64, 57], [130, 86, 70], [183, 120, 98], [125, 89, 93],
        [83, 59, 65], [63, 51, 59], [43, 34, 42], [109, 78, 75], [134, 112, 102],
        [180, 157, 126], [196, 198, 184]
    ]

    # Charger l'image
    img = Image.open(image_path).convert("RGB")

    # Redimensionner l'image à la taille demandée
    img = img.resize((largeur_cible, hauteur_cible), Image.LANCZOS)
    pixels = img.load()

    im, c, prec = "", 0, None

    for y in range(hauteur_cible):
        for x in range(largeur_cible):
            v = pixels[x, y]
            index_couleur = couleur_proche(v, palette_fixe)

            if prec is None:
                prec = index_couleur

            if prec != index_couleur:
                im += lett[prec] + (str(c) if c > 1 else "")
                c, prec = 1, index_couleur
            else:
                c += 1

    im += lett[prec] + (str(c) if c > 1 else "")
    return im

def main():
    print(f"🔍 Le script va analyser le dossier '{DOSSIER_INPUT}' et convertir les images en texte.")
    print(f"⚠️ Toutes les anciennes données dans '{DOSSIER_OUTPUT}' seront supprimées.")
    confirmation = input("Voulez-vous continuer ? (o/N) ").strip().lower()
    
    if confirmation != "o":
        print("❌ Opération annulée.")
        return

    # Nettoyer le dossier output
    nettoyer_output()

    # Récupérer la liste des images
    images = trouver_images(DOSSIER_INPUT)

    if not images:
        print("❌ Aucune image trouvée dans le dossier input.")
        return

    if len(images) == MAX_IMAGES:
        confirmation = input(f"⚠️ {MAX_IMAGES} images trouvées. Voulez-vous continuer ? (o/N) ").strip().lower()
        if confirmation != "o":
            print("❌ Opération annulée.")
            return

    contenu_output = ""

    for fichier in images:
        # Remplacement des espaces par des tirets
        nom_image_sans_ext = os.path.splitext(fichier)[0].replace(" ", "-")
        image_path = os.path.join(DOSSIER_INPUT, fichier)

        # Charger l'image pour obtenir ses dimensions actuelles
        img = Image.open(image_path)
        largeur_actuelle, hauteur_actuelle = img.size

        # Demander la nouvelle taille
        largeur_cible, hauteur_cible = demander_dimensions(fichier, largeur_actuelle, hauteur_actuelle)

        print(f"🖼️ Conversion de {fichier}...")

        texte_image = image_en_texte(image_path, largeur_cible, hauteur_cible)

        # Nom du fichier texte avec taille dans src/
        text_filename = f"{nom_image_sans_ext}-{largeur_cible}x{hauteur_cible}.txt"
        text_path = os.path.join(DOSSIER_SRC, text_filename)

        # Sauvegarde du texte dans /output/src
        with open(text_path, "w", encoding="utf-8") as f:
            f.write(texte_image)

        # Ajouter au fichier output.txt
        contenu_output += f"{nom_image_sans_ext} {largeur_cible} {hauteur_cible}\n{texte_image}\n\n\n"

    # Écriture du fichier output.txt
    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        f.write(contenu_output)

    print(f"\n✅ Conversion terminée. Résultat enregistré dans {OUTPUT_FILE}")

if __name__ == "__main__":
    main()
