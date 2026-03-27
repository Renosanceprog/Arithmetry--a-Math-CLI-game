# 🕹️🔢 Arithmetry!

A fully-featured, fast-paced mathematical arcade game built entirely for the Windows Command Prompt. It features dynamic difficulty, boss fights, a completely custom audio engine, and a fully reactive ASCII user interface.

## ✨ Features

### 👤 Authentication & Profiles
* **Local Account System:** Fully functional Login and Register flow with duplicate username checks and password confirmation.
* **Persistent Save Files:** Automatic file handling ensures your stats, high scores, and user profiles are saved locally.
* **Global Statistics:** Tracks lifetime metrics including Total Correct/Wrongs (K/D), Highest Streak, and Fastest Answer Time.

### 🎮 Gameplay Mechanics
* **Four Core Operations:** Choose to practice Addition, Subtraction, Multiplication, or Division.
* **Dynamic Difficulty:** The game gets progressively harder every 10 questions, modifying the min/max values of the problem generator. Time multipliers automatically adjust to keep difficulty spikes fair.
* **Boss Fights:** Every 10th question triggers a "Boss Problem" that combines multiple operations into a single equation.
* **Streak & Multiplier System:** Maintain a streak (starting at 5 questions) to activate a continuous 1.1x point multiplier.
* **Tiered Scoring:** Points are awarded based on speed and streaks, with visual and audio hype for "Quick," "Godlike," and "Streak Burst" answers.

### 🏆 Leaderboards & Progression
* **High Score Tracking:** The game tracks and saves your personal best scores. 
* **Dynamic Leaderboard:** View the overall global standings or filter by specific mathematical operations. Only top scores are displayed to prevent leaderboard clutter.
* **Post-Game Summary:** Features a satisfying animated "score reel" effect that tallies up your final score and displays overall standings.

### 🖥️ Graphics & UI
* **Non-Blocking Menus:** Navigate smoothly using Arrow Keys, Enter to confirm, and ESC to back out of any menu.
* **Fixed Window ASCII Graphics:** Runs in a controlled terminal window with centered text formatting and custom ASCII block borders.
* **Reactive Environment:** The terminal border dynamically changes color as your streak increases.
* **Boss Room Immersion:** The entire UI shifts to an aggressive Red theme when a Boss Problem appears.

### 🎵 Custom Audio Engine
Features a custom, multi-threaded 8-bit audio engine built using the Windows API (`Beep`), ensuring sounds never freeze the gameplay loop.
* **UI/Menu SFX:** Mechanical keyboard typing clicks, blocked input thuds, success chimes, invalid buzzers, and menu navigation blips.
* **Gameplay SFX:** Tiered correct answer coins (Normal, Quick, Godlike), streak burst power-ups, wrong answer buzzers, heavy boss alarms, boss defeat fanfares, and a classic game over jingle.
* **Progression SFX:** High-speed score reveal ticking and triumphant "New High Score" fanfares.

---

## 🛠️ Bug Fixes & Optimizations (Latest Patch)
* Refactored UI elements to be perfectly centered across Stats, Leaderboards, Lobbies, and the Game Loop.
* Handled invalid/empty inputs during the `askQuestion` phase to prevent crashes.
* Fixed the "double Enter" bug in the Play category menu.
* Re-mapped the Play category to correctly accept ESC to back out.
* Moved the "Invalid Option" warning to cleanly display below dropdown menus.

---

## 🚀 Roadmap (Upcoming Features)

- [ ] **In-Game Currency System:** Earn tokens by playing games and answering questions correctly.
- [ ] **Item Shop:** Spend currency on power-ups that can be used mid-game (via `kbhit` detection):
  - [ ] 🛡️ *Wrong Shield:* Protects your streak and life from a wrong answer.
  - [ ] 🎲 *Reroll Question:* Swaps out a difficult equation for a new one.
  - [ ] ⏭️ *Skip Question:* Skips the problem entirely while still awarding base points.
- [ ] **Premium Gamemodes**
  - [ ] ⏱️ Time Attack ("60-Second Blitz")
  - [ ] 🙈 Memory Math ("Blindfold Mode")
  - [ ] ☠️ The Gauntlet ("Boss Rush")
  - [ ] 🎰 The Casino ("Wager Mode")
- [ ] **Post-Death Roulette:** A chance to spin a wheel for a bonus after a game over.
- [ ] **1-Up Mechanic:** Achieving a 15-question streak automatically restores 1 life.
- [ ] **Unlockable Game Modes:** New players receive a starter allowance of currency to try premium modes. Once depleted, players must farm tokens in "Classic Mode" to afford entry into special game modes bought from the shop.

## 🚀 Getting Started

### Prerequisites
This game is built natively for the **Windows Command Prompt** to take advantage of low-level system APIs. To compile and run from source, you will need:
* **OS:** Windows 10 or 11 (Required for `<windows.h>` and modern ANSI escape sequence support).
* **Compiler:** A C compiler like GCC (via MinGW) installed and added to your system PATH.

### Compilation & Installation
1. **Clone the repository:**
   ```bash
   git clone [https://github.com/YourUsername/arithmetic-arcade-game.git](https://github.com/YourUsername/arithmetic-arcade-game.git)
   cd arithmetic-arcade-game
   ```

2. **Run the Game:**
   Double-click `MathArcade.exe` in your File Explorer, or launch it directly from the terminal:
   ```bash
   ./MathArcade.exe
   ```

---

## 🧠 Implementation & Architecture (Under the Hood)

This project is built purely in **C** with zero external game engines or graphics libraries (No SDL, Raylib, etc.). It relies entirely on the native Windows API, terminal manipulation, and custom algorithms.

* **Async Audio Engine:** Features a custom-built, multi-threaded sound engine using `CreateThread` and the native Windows `Beep()` function. This delegates sound frequencies to background threads, allowing for instantaneous audio feedback and complex jingles without ever blocking or slowing down the main gameplay loop.
* **Non-Blocking UI:** Utilizes `_getch()` for raw, instantaneous keyboard polling. This allows for fluid, arrow-key menu navigation and mechanical typing feedback without requiring the `Enter` key to submit every keystroke.
* **Dynamic Graphics & Rendering:** The UI engine is driven by raw **ANSI Escape Codes** (`\e[38;5;Xm`). It utilizes 256-color palettes, custom ASCII geometry for fixed-window borders, and localized cursor manipulation (`\033[Y;XH`) to update specific UI elements (like timers or score reels) without flickering the screen.
* **Memory Management:** User accounts, local leaderboards, and session data are managed entirely in memory using **Linked Lists**. This allows for scalable local profile creation using dynamic memory allocation (`calloc`/`free`) while preventing memory leaks.
* **Persistent Storage:** A custom file-handling system automatically serializes and deserializes the Linked List structs to local data files (`.txt`/`.dat`), ensuring seamless profile loading, streak tracking, and high-score saving between sessions.
