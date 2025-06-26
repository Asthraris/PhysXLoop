# 🧪 PhysXLoop – C++ Physics Engine on OpenGL 4.6

**PhysXLoop** is a physics-first simulation engine built from scratch in modern C++ using **OpenGL 4.6** for rendering and **Eigen** for mathematical precision. It’s designed for accurate, real-time physical interaction — with core focus on robust **collision detection/resolution**, a **modular entity model**, and an **event-controlled runtime** that enables seamless switching between simulation and camera control.

---

## 🧠 Core Idea

Unlike traditional render-first engines, PhysXLoop prioritizes **physics determinism and logical accuracy** over GPU performance. While it integrates rendering using OpenGL, its strength lies in a cleanly separated **physics loop** that runs in sync (or optionally detached) from the rendering loop using a lambda-controlled runner.

---

## ⚙️ Features

### ✅ Physics Engine

* 🧱 **AABB–AABB Collision Detection + Resolution**
* 🟣 **Sphere–Sphere Collision Handling**
* ⚡ **Elastic & Inelastic Collision Types**
* 🌌 **Accurate Penetration Depth Handling**
* 🔃 **Deterministic Collision Resolution Logic**
* 🔂 **Toggleable Physics Loop (Pause/Resume)** via space key

### 🎮 Simulation Loop

* 🎯 **Lambda-based Loop Binding** for physics and rendering sync
* ⏸️ **Event-controlled Simulation State**
* 🖱️ **Arcball-style Camera Control** active when simulation is paused

### 🖼️ Rendering + UI

* 📦 **Modular Renderer (Custom-built)**
* 🔢 **ImGui Docking UI Integration**
* 🎲 **Visual Debug Tools** for viewing entities & bounding volumes
* 🧩 **Custom OOP Entity System** (vs ECS used in prior engine)

---

## 🧰 Tech Stack

| Component      | Stack                      |
| -------------- | -------------------------- |
| Renderer       | OpenGL 4.6                 |
| Math Engine    | Eigen                      |
| GUI            | Dear ImGui (Docking)       |
| Entity System  | Custom OOP-based Model     |
| Loop Control   | Lambda-driven loop binding |
| Input Handling | GLFW                       |
| Build System   | CMake                      |

---

## 🔧 Controls

| Key     | Action                          |
| ------- | ------------------------------- |
| `Space` | Pause / Resume Physics Loop     |
| `Mouse` | Arcball-style camera (on pause) |
| `Esc`   | Exit Simulation                 |
| `Tab`   | Toggle ImGui Dock               |

---

## 🧪 Architecture Highlights

* **PhysicsEngine.run()** is passed as a lambda to the main loop, allowing full detachment if needed.
* Clear separation between `Renderer`, `Entity`, `CollisionSolver`, and `PhysicsEngine` modules.
* Minimal ImGui overlay for controlling simulation state and inspecting runtime data.

---

## 📈 Roadmap

* [ ] Dynamic Object Creation via GUI
* [ ] Particle Systems (Boids, Verlet)
* [ ] Fluid Simulation (SPH-based or grid-based)
* [ ] Rigid Body Dynamics
* [ ] Advanced constraint solver (springs, hinges)
* [ ] Performance profiling & GPU-acceleration

---

## 🧪 Example Simulation

> **Sphere–Sphere Collision Demo**
> Two spheres collide elastically with position correction and velocity exchange. Penetration is resolved using impulse-based solver.

> **AABB Stack Stress Test**
> Multiple stacked AABBs fall and resolve collisions, showcasing broad-phase + narrow-phase handling.

---

## 🪪 License

MIT License – open for educational, academic, and commercial experimentation.

---

## 👨‍💻 Author

**Aman Gupta**
Driven by physics. Rendered with code.
*From graphics to gravity — one frame at a time.*
