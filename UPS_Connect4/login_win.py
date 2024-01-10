import socket
import threading
import time
from tkinter import ttk
import tkinter as tk
from tkinter import messagebox
import validation as valid
import send_messages as message_handler
from game_win import GameWindow
from lobby_win import LobbyWin

DISS_TIMER = 100
class LoginWin:
    def __init__(self, root):
        self.name = None
        self.root = root
        self.root.title("Connect4 - Login")
        self.server_socket = None
        self.thread = None
        self.lobby_listbox = None
        self.lobby_win_initializer = None
        self.game_win_initializer = None
        self.buffer_size = 1024
        self.buffer = b''
        self.open_lobby_flag = False
        self.number_game = 0
        self.opponent = None
        self.move = None
        self.color = None
        self.mark = None
        self.priority = 0
        self.move_opponent = None
        self.color_opponent = None
        self.mark_opponent = None
        self.end_message = None
        self.last_message_time = 0
        self.timer_thread = None
        self.timer_stop_event = None
        self.thread_locker = threading.Lock()
        self.state_opponent = ""
        self.serializedData = ""

        self.login_frame = ttk.Frame(self.root)
        self.login_frame.pack(padx=10, pady=10)

        self.server_ip = ttk.Label(self.login_frame, text="IP adresa serveru:")
        self.server_ip.pack()
        self.ip_entry = ttk.Entry(self.login_frame)
        self.ip_entry.pack()

        self.server_port = ttk.Label(self.login_frame, text="Port serveru:")
        self.server_port.pack()
        self.port_entry = ttk.Entry(self.login_frame)
        self.port_entry.pack()

        self.name_label = ttk.Label(self.login_frame, text="Vaše jméno:")
        self.name_label.pack()

        self.name_entry = ttk.Entry(self.login_frame)
        self.name_entry.pack()

        self.connect_button = ttk.Button(self.login_frame, text="Připojit", command=self.on_login)
        self.connect_button.pack()

    def end(self):
        self.root.destroy()

    def on_login(self):
        ip = self.ip_entry.get()
        port = int(self.port_entry.get())
        name = self.name_entry.get()

        if not (valid.validate_ip(ip) and valid.validate_port(port)
                and valid.validate_name(name)):
            return

        login_message = message_handler.login_message(name)

        self.setup_network(ip, port, name, login_message)

    def setup_network(self, ip, port, name, login_message):
        try:
            self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server_socket.connect((ip, port))
            self.name = name

            self.thread = threading.Thread(target=self.listen_to_server)
            self.thread.daemon = True
            self.thread.start()

            self.server_socket.sendall(login_message.encode())
            self.last_message_time = time.time()

            self.timer_stop_event = threading.Event()
            self.timer_thread = threading.Thread(target=self.timeout)
            self.timer_thread.daemon = True
            self.timer_thread.start()

            self.root.withdraw()


        except Exception as e:
            print(f"Chyba při připojování k serveru: {e}")

    def listen_to_server(self):
        while True:
            try:
                data = self.server_socket.recv(self.buffer_size)
                self.last_message_time = time.time()
                if not data:
                    print("Spojení se serverem bylo ukončeno.")
                    break

                self.buffer += data
                messages = self.buffer.split(b'\n')
                self.buffer = messages.pop()

                for msg in messages:
                    msg = msg.decode()
                    self.handle_response(msg)

            except Exception as e:
                print(f"Chyba při čtení ze serveru: {e}")
                break

    def timeout(self):
        while not self.timer_stop_event.is_set():
            with self.thread_locker:
                current_time = time.time()
                elapsed_time = current_time - self.last_message_time
                if elapsed_time >= DISS_TIMER:
                    self.connection_lost()
                    return

            time.sleep(3)

    def connection_lost(self):

        info_message = ("Lost connection with server!")
        alert_window = tk.Toplevel(self.root)
        alert_window.title("Connection lost!")

        label = ttk.Label(alert_window, text=info_message)
        label.pack(padx=10, pady=10)

        self.lobby_win_initializer.destroy_lobby_window()
        if self.game_win_initializer is not None:
            self.game_win_initializer.destroy_window()
        try:
            if self.server_socket:
                self.server_socket.close()
        except Exception as e:
            print(f"Killing: {e}")
        self.server_socket = None
        self.root.deiconify()
        self.timer_stop_event.set()

    def open_lobby_window(self, server):
        self.lobby_win_initializer = LobbyWin(self.root, server, self.name)

    def handle_response(self, response):
        self.last_message_time = time.time()
        print("SERVER:", response)
        response = response.replace("\n", "")
        responses = message_handler.parser(response)
        if message_handler.message_valid(responses, self.name):
            self.handle_message(responses)
        else:
            print("Message is invalid.")
        pass

    def handle_message(self, messages):
        if messages[0] == "LOGIN":
            if messages[1] == "ERR":
                messagebox.showerror(messages[2])
        elif messages[1] == "PING":
            ping_mess = message_handler.ping_message(self.name)
            self.server_socket.sendall(ping_mess.encode())
            print(ping_mess)
        elif messages[1] == "ERR":
            messagebox.showerror(messages[1], messages[2] + " " + messages[3] + " " + messages[4])
        elif messages[1] == "LOGIN":
            if messages[2] == "OK":
                self.open_lobby_window(self.server_socket)
        elif messages[1] == "FIND_GAME":
            if messages[2] == "OK":
                self.number_game = messages[4]
                self.mark = messages[5]
                if messages[5] == "X":
                    self.color = "red"
                elif messages[5] == "O":
                    self.color = "yellow"
                self.lobby_win_initializer.update_info_game(self.number_game)
        elif messages[1] == "OPPONENT":
            self.opponent = messages[2]
            self.mark_opponent = messages[3]
            self.state_opponent = messages[4]
            if messages[3] == "X":
                self.color_opponent = "red"
            elif messages[3] == "O":
                self.color_opponent = "yellow"
            self.lobby_win_initializer.update_info_opponent(self.opponent)
            self.lobby_win_initializer.info_opp(self.state_opponent)
        elif messages[1] == "START_GAME":
            if messages[2] == "OK":
                self.lobby_win_initializer.close_lobby_window()
                self.game_win_initializer = GameWindow(self.root, self.server_socket,
                                                       self.lobby_win_initializer.lob_window, self.name)
                self.game_win_initializer.players_info(self.name, self.color, self.opponent,
                                                       self.color_opponent)
                self.game_win_initializer.priority_info(self.priority)
                self.game_win_initializer.info_opp(self.state_opponent)
        elif messages[1] == "GAME_MOVE":
            if messages[2] == "OK":
                self.move = int(messages[3])
                self.priority -= 1
                print(self.color)
                print(self.mark)
                self.game_win_initializer.move(self.move, self.color, self.mark)
                self.game_win_initializer.priority_info(self.priority)
        elif messages[1] == "IT_IS_YOUR_TURN":
            if messages[2] == "1":
                self.priority = int(messages[2])
                self.game_win_initializer.priority_info(self.priority)
            elif messages[2] == "OPPONENT_TURN":
                self.move_opponent = int(messages[3])
                self.priority += 1
                self.game_win_initializer.move(self.move_opponent, self.color_opponent, self.mark_opponent)
                self.game_win_initializer.priority_info(self.priority)
        elif messages[1] == "GAME_END":
            self.end_message = messages[2]
            self.game_win_initializer.info_end(self.end_message)
            self.game_win_initializer.off_button()
            self.opponent = None
            self.number_game = 0
            self.lobby_win_initializer.update_info_game(self.number_game)
            self.lobby_win_initializer.update_info_opponent(self.opponent)
        elif messages[1] == "LEAVE_GAME":
            if messages[2] == "OK":

                if self.game_win_initializer is not None:
                    self.game_win_initializer.close_window()

                self.opponent = None
                self.number_game = 0
                self.state_opponent = ""
                self.lobby_win_initializer.info_opp(self.state_opponent)
                self.lobby_win_initializer.update_info_game(self.number_game)
                self.lobby_win_initializer.update_info_opponent(self.opponent)
        elif messages[1] == "LOGOUT":
            if messages[2] == "OK":
                if self.game_win_initializer is not None:
                    self.game_win_initializer.destroy_window()
                if self.lobby_win_initializer is not None:
                    self.lobby_win_initializer.destroy_lobby_window()

                self.root.deiconify()
        elif messages[1] == "INFO":
            if messages[3] == "DEQUEUE":
                self.mark = messages[4]
                if messages[4] == "X":
                    self.color = "red"
                elif messages == "O":
                    self.color = "yellow"
                self.opponent = None
                self.lobby_win_initializer.update_info_opponent(self.opponent)

            if messages[3] == "CONN":
                self.state_opponent = messages[3]
                if self.game_win_initializer is not None:
                    self.game_win_initializer.info_opp(self.state_opponent)
                self.lobby_win_initializer.info_opp(self.state_opponent)

            if messages[3] == "DISCON":
                self.state_opponent = messages[3]
                if self.game_win_initializer is not None:
                    self.game_win_initializer.info_opp(self.state_opponent)
                self.lobby_win_initializer.info_opp(self.state_opponent)

        elif messages[1] == "RECONNECT":
            if messages[2] == "OK":
                if messages[3] == "GAME":
                    self.number_game = messages[4]
                    self.mark = messages[5]
                    self.opponent = messages[6]
                    self.mark_opponent = messages[7]
                    self.priority = int(messages[8])
                    self.state_opponent = messages[9]
                    self.open_lobby_window(self.server_socket)
                    self.lobby_win_initializer.info_opp(self.state_opponent)
                    self.lobby_win_initializer.update_info_opponent(self.opponent)
                    self.lobby_win_initializer.update_info_game(self.number_game)
                    if messages[5] == "X":
                        self.color = "red"
                    elif messages[5] == "O":
                        self.color = "yellow"
                    if messages[7] == "X":
                        self.color_opponent = "red"
                    elif messages[7] == "O":
                        self.color_opponent = "yellow"
                else:
                    self.mark = messages[3]
                    self.opponent = messages[4]
                    self.mark_opponent = messages[5]
                    self.state_opponent = messages[6]
                    if messages[3] == "X":
                        self.color = "red"
                    elif messages[3] == "O":
                        self.color = "yellow"
                    if messages[5] == "X":
                        self.color_opponent = "red"
                    elif messages[5] == "O":
                        self.color_opponent = "yellow"

                    self.priority = int(messages[7])

                    self.lobby_win_initializer = LobbyWin(self.root, self.server_socket, self.name)
                    self.lobby_win_initializer.close_lobby_window()
                    self.game_win_initializer = GameWindow(self.root, self.server_socket,
                                                           self.lobby_win_initializer.lob_window, self.name)
                    array = self.game_win_initializer.deserialize(messages[8], 6, 7)
                    self.game_win_initializer.update_game_array(array)
                    self.game_win_initializer.players_info(self.name, self.color, self.opponent,
                                                           self.color_opponent)
                    self.game_win_initializer.priority_info(self.priority)
                    self.game_win_initializer.info_opp(self.state_opponent)






