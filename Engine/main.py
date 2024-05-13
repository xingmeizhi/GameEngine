from tkinter import ttk
import mygameengine
import tkinter as tk


class MapEditor:
    """
    \class MapEditor
    \brief A graphical map editor for creating and modifying game levels.

    This class provides a GUI for adding, moving, and deleting 'enemy' and 'food' items on a game level. Users can select levels to edit, add enemies and foods, drag these items around the canvas, and save their configurations.
    """

    def __init__(self, root):
        """
        \brief Initialize the MapEditor with a given Tkinter root.
        \param root The Tkinter root widget.
        """
        self.root = root
        self.drag_data = {"x": 0, "y": 0, "item": None}
        self.enemies = {}
        self.enemy_id = 0
        self.foods = {}
        self.food_id = 0
        self.selected_enemy = None
        self.selected_food = None
        self.current_level = tk.StringVar(value="level1")
        self.init_ui()
        self.load_level()

    def init_ui(self):
        """
        \brief Initializes the user interface components for the map editor.
        """
        self.root.title("Map Editor")
        self.canvas_width = 640
        self.canvas_height = 480
        self.canvas = tk.Canvas(self.root,
                                width=self.canvas_width,
                                height=self.canvas_height)
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.background_image = tk.PhotoImage(file="assets/scene.png")
        self.canvas.create_image(0,
                                 0,
                                 anchor=tk.NW,
                                 image=self.background_image)
        self.enemy_button = tk.Button(self.root,
                                      text="Add Enemy",
                                      command=self.add_enemy)
        self.enemy_button.pack(side=tk.LEFT)

        self.food_button = tk.Button(self.root,
                                     text="Add Food",
                                     command=self.add_food)
        self.food_button.pack(side=tk.LEFT)

        self.save_button = tk.Button(self.root,
                                     text="Save Config",
                                     command=self.save_enemies_and_foods)
        self.save_button.pack(side=tk.LEFT)

        self.delete_button = tk.Button(self.root,
                                       text="Delete Selected",
                                       command=self.delete_selected)
        self.delete_button.pack(side=tk.LEFT)

        self.level_selector = ttk.Combobox(
            self.root,
            textvariable=self.current_level,
            values=["level1", "level2", "level3"])
        self.level_selector.pack(side=tk.TOP)
        self.level_selector.bind("<<ComboboxSelected>>", self.load_level)

        self.canvas.bind("<Button-1>", self.on_drag_start)
        self.canvas.bind("<B1-Motion>", self.on_drag_move)
        self.canvas.bind("<ButtonRelease-1>", self.on_drag_stop)

    def load_level(self, event=None):
        """
        \brief Loads the enemies and foods from a configuration file for the current level.
        \param event Optional event parameter for Tkinter event handling compatibility.
        """
        level = self.current_level.get()
        config_file = f"config/{level}_config.txt"
        self.enemies.clear()
        self.foods.clear()
        self.enemy_id = 0
        self.food_id = 0
        self.canvas.delete("enemy")
        self.canvas.delete("food")
        with open(config_file, 'r') as file:
            lines = file.readlines()
            mode = None
            for i in range(len(lines)):
                line = lines[i].strip()
                if "Enemies Configuration" in line:
                    mode = "enemy"
                elif "Foods Configuration" in line:
                    mode = "food"
                elif line:
                    tag, value = line.split("=")
                    if "_x" in tag:
                        y_line = lines[i + 1].strip()
                        _, y_value = y_line.split("=")
                        x, y = float(value), float(y_value)
                        if mode == "enemy":
                            self.add_enemy(x, y, tag.replace("_x", ""))
                        elif mode == "food":
                            self.add_food(x, y, tag.replace("_x", ""))

    def add_enemy(self, x=0, y=0, tag=None):
        """
        \brief Adds an enemy to the canvas at the specified location.
        \param x The x-coordinate on the canvas where the enemy should be placed.
        \param y The y-coordinate on the canvas where the enemy should be placed.
        \param tag An optional unique identifier for the enemy. If none is provided, an id is generated.
        """
        self.enemy_id += 1
        enemy_image = tk.PhotoImage(file="assets/enemy.png")
        if tag is None:
            tag = "enemy" + str(self.enemy_id)
        item = self.canvas.create_image(x,
                                        y,
                                        anchor=tk.NW,
                                        image=enemy_image,
                                        tags=(tag, ))
        self.enemies[tag] = {
            "image": enemy_image,
            "item": item,
            "x": x,
            "y": y
        }

    def add_food(self, x=0, y=0, tag=None):
        """
        \brief Adds a food item to the canvas at the specified location.
        \param x The x-coordinate on the canvas where the food should be placed.
        \param y The y-coordinate on the canvas where the food should be placed.
        \param tag An optional unique identifier for the food. If none is provided, an id is generated.
        """
        self.food_id += 1
        food_image = tk.PhotoImage(file="assets/food.png")
        if tag is None:
            tag = "food" + str(self.food_id)
        item = self.canvas.create_image(x,
                                        y,
                                        anchor=tk.NW,
                                        image=food_image,
                                        tags=(tag, ))
        self.foods[tag] = {"image": food_image, "item": item, "x": x, "y": y}

    def on_drag_start(self, event):
        """
        \brief Initiates dragging of an enemy or food item on the canvas.
        \param event The Tkinter event containing the mouse coordinates.
        """
        item = self.canvas.find_closest(event.x, event.y)[0]
        tags = self.canvas.gettags(item)
        for tag in tags:
            if tag.startswith(("enemy", "food")):
                self.drag_data["item"] = item
                self.drag_data["x"] = event.x
                self.drag_data["y"] = event.y
                self.selected_enemy = item if "enemy" in tag else None
                self.selected_food = item if "food" in tag else None
                return

    def on_drag_move(self, event):
        """
        \brief Handles the dragging movement of an item.
        \param event The Tkinter event containing the new mouse coordinates.
        """
        dx = event.x - self.drag_data["x"]
        dy = event.y - self.drag_data["y"]
        if self.drag_data["item"] is not None:
            self.canvas.move(self.drag_data["item"], dx, dy)
            self.drag_data["x"] = event.x
            self.drag_data["y"] = event.y

    def on_drag_stop(self, event):
        """
        \brief Finalizes the drag operation, updating the item's position.
        \param event The Tkinter event signaling the release of the mouse button.
        """
        if self.drag_data["item"] is not None:
            coords = self.canvas.coords(self.drag_data["item"])
            tag = self.canvas.gettags(self.drag_data["item"])[0]
            if tag.startswith("enemy"):
                self.enemies[tag]["x"] = coords[0]
                self.enemies[tag]["y"] = coords[1]
                print(
                    f"New position - enemy{self.enemy_id} X: {coords[0]}, Y: {coords[1]}"
                )
            elif tag.startswith("food"):
                self.foods[tag]["x"] = coords[0]
                self.foods[tag]["y"] = coords[1]
                print(
                    f"New position - food{self.food_id} X: {coords[0]}, Y: {coords[1]}"
                )
            self.drag_data["item"] = None

    def delete_selected(self):
        """
        \brief Deletes the currently selected enemy or food item from the canvas.
        """
        if self.selected_enemy:
            tag = self.canvas.gettags(self.selected_enemy)[0]
            if tag in self.enemies:
                del self.enemies[tag]
            self.canvas.delete(self.selected_enemy)
            self.selected_enemy = None
            self.reassign_ids(self.enemies, "enemy")
        elif self.selected_food:
            tag = self.canvas.gettags(self.selected_food)[0]
            if tag in self.foods:
                del self.foods[tag]
            self.canvas.delete(self.selected_food)
            self.selected_food = None
            self.reassign_ids(self.foods, "food")

    def reassign_ids(self, items, prefix):
        """
        \brief Reassigns IDs to items in the canvas, useful after deletion operations.
        \param items The dictionary of items (either enemies or foods) to reassign.
        \param prefix The prefix string used to generate new tags for items.
        """
        new_id = 1
        for tag in list(items.keys()):
            new_tag = f"{prefix}{new_id}"
            items[new_tag] = items.pop(tag)
            items[new_tag]['item'] = self.canvas.itemconfigure(
                items[new_tag]['item'], tags=(new_tag, ))
            new_id += 1
        if prefix == "enemy":
            self.enemy_id = new_id - 1
        else:
            self.food_id = new_id - 1

    def save_enemies_and_foods(self):
        """
        \brief Saves the current configuration of enemies and foods to a file.
        """
        config_filename = f"config/{self.current_level.get()}_config.txt"
        with open(config_filename, 'w') as config_file:
            config_file.write("# Enemies Configuration\n")
            for tag, info in self.enemies.items():
                config_file.write(f"{tag}_x={info['x']}\n")
                config_file.write(f"{tag}_y={info['y']}\n")
            config_file.write("# Foods Configuration\n")
            for tag, info in self.foods.items():
                config_file.write(f"{tag}_x={info['x']}\n")
                config_file.write(f"{tag}_y={info['y']}\n")
        print("config saved")


def main():
    root = tk.Tk()
    editor = MapEditor(root)
    root.mainloop()
    editor.save_enemies_and_foods()
    game = mygameengine.Application(640, 480)
    game.loop(60.0)


if __name__ == "__main__":
    main()
