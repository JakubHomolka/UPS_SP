import tkinter as tk
import send_messages as message_handler

class GameWindow:
    def __init__(self, parent, server, lob_window, name):
        self.opponent_color_label = None
        self.opponent_label = None
        self.opponent_name_label = None
        self.color_label = None
        self.color = None
        self.you_label = None
        self.name_label = None
        self.priority_label = None
        self.game_end_window = None
        self.logout_game_button = None
        self.leave_game_button = None
        self.info_opponent = None
        self.message_label = None
        self.column_buttons = []
        self.parent = parent
        self.server = server
        self.name = name
        self.opponent_color = None
        self.lob_window = lob_window
        self.game_window = tk.Toplevel(parent)
        self.game_window.title("Connect4 - Game")
        self.end_message = None
        self.game_window.protocol("WM_DELETE_WINDOW", self.end)

        self.game_state = [[" " for _ in range(7)] for _ in range(6)]  # 2D pole pro stav hry
        self.grid_canvases = [[None for _ in range(7)] for _ in range(6)]  # GUI reprezentace
        self.grid_circles = [[None for _ in range(7)] for _ in range(6)]

        self.setup_player_info()

        self.setup_game_grid()

    def end(self):
        self.parent.destroy()

    def setup_player_info(self):

        self.you_label = tk.Label(self.game_window, text="You: ")
        self.you_label.grid(row=0, column=0, sticky='w')

        self.name_label = tk.Label(self.game_window, text="Name: ")
        self.name_label.grid(row=0, column=1, sticky='w')

        self.color_label = tk.Label(self.game_window, text="Color: ")
        self.color_label.grid(row=1, column=1, sticky='w')

        self.priority_label = tk.Label(self.game_window, text="Priority: ")
        self.priority_label.grid(row=2, column=1, sticky='w')

        self.message_label = tk.Label(self.game_window, text="")
        self.message_label.grid(row=2, column=3)

        self.opponent_label = tk.Label(self.game_window, text="Opponent: ")
        self.opponent_label.grid(row=0, column=5, sticky='w')

        self.opponent_name_label = tk.Label(self.game_window, text="Name: ")
        self.opponent_name_label.grid(row=0, column=6, sticky='w')

        self.opponent_color_label = tk.Label(self.game_window, text="Color: ")
        self.opponent_color_label.grid(row=1, column=6, sticky='w')

        self.info_opponent = tk.Label(self.game_window, text="State: ")
        self.info_opponent.grid(row=2, column=6, sticky='w')

        self.leave_game_button = tk.Button(self.game_window, text="Leave game", command=self.leave_message)
        self.leave_game_button.grid(row=0, column=3)

    def leave_message(self):
        leave_game_message = message_handler.leave_game_message(self.name)
        self.server.sendall(leave_game_message.encode())

    def info_opp(self, state):
        self.info_opponent.config(text=f"State: {state}")

    def info_end(self, end):
        self.message_label.config(text=f"State: {end}")

    def setup_game_grid(self):
        # Vytvoření herní mřížky
        for row in range(6):
            for col in range(7):
                # Vytvoření instance Canvas namísto Label
                canvas = tk.Canvas(self.game_window, width=90, height=90, bg="blue", highlightthickness=1,
                                   relief="solid")
                canvas.grid(row=row + 5, column=col, sticky='w')

                # Nakreslení kruhu (žetonu) na Canvas
                circle = canvas.create_oval(5, 5, 85, 85, fill="white", outline="blue")
                self.grid_circles[row][col] = circle
                self.grid_canvases[row][col] = canvas

        # Přidání tlačítek pro volbu sloupce
        for col in range(7):
            button = tk.Button(self.game_window, text=f"Column {col + 1}",
                               command=lambda c=col + 1: self.move_message(c), state=tk.NORMAL)
            button.grid(row=12, column=col)
            self.column_buttons.append(button)

    def move_message(self, col):
        move_message = message_handler.move_message(self.name, col)
        self.server.sendall(move_message.encode())

    def players_info(self, name, color, opponent_name, opponent_color):

        self.name_label.config(text=f"Name: {name}")

        self.color_label.config(text=f"Color: {color}")
        self.color = color

        self.opponent_name_label.config(text=f"Name: {opponent_name}")

        self.opponent_color_label.config(text=f"Color: {opponent_color}")
        self.opponent_color = opponent_color

    def priority_info(self, priority):

        self.priority_label.config(text=f"Priority: {priority}")

    def move(self, col, color, mark):
        col = col - 1
        print(color)
        print(mark)
        # Logika pro přidání žetonu do sloupce
        for row in range(5, -1, -1):  # Procházíme sloupec odspodu
            if self.game_state[row][col] == " ":
                # Aktualizujeme stav hry
                print("mark")
                self.game_state[row][col] = mark
                # Aktualizujeme GUI
                print("update")
                self.update_button(row, col, color)  #
                break

    def off_button(self):
        for button in self.column_buttons:
            button['state'] = tk.DISABLED

    def update_game_array(self, array):
        for row in range(6):
            for col in range(7):
                self.game_state[row][col] = array[row][col]
                if self.game_state[row][col] == "X":
                    self.update_button(row, col, "red")
                elif self.game_state[row][col] == "O":
                    self.update_button(row, col, "yellow")

    def deserialize(self, serialized_data, rows, cols):
        # Rozdělení řetězce na jednotlivé prvky
        elements = serialized_data.split(';')

        # Odstranění posledního prázdného prvku vzniklého kvůli závěrečnému středníku
        elements = elements[:-1]

        # Převedení jednotlivých prvků na 2D pole
        return [elements[i * cols:(i + 1) * cols] for i in range(rows)]

    def update_button(self, row, col, color):
        print(color)
        canvas = self.grid_canvases[row][col]
        circle = self.grid_circles[row][col]
        canvas.itemconfig(circle, fill=color)

    def close_window(self):
        self.game_window.destroy()
        self.lob_window.deiconify()

    def destroy_window(self):
        self.game_window.destroy()


