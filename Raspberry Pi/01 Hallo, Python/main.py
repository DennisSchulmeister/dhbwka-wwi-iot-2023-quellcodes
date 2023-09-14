def main():
    name = ""

    while not name:
        name = input("Wie heißt du? ")

    print(f"Hallo, {name}!")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
