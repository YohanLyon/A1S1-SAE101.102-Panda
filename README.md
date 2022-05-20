# A1S1-SAE101.102

SAE of semester number 1 on the theme of programming
> Programming of an application allowing the analysis of the bamboo shoot. It is a simulation using a panda robot. That gives the statistics.</br>

## Topic

You just bought a house by a lake. A bamboo garden grows outside the house and obstructs the beautiful view of the lake. To solve this problem, you buy a panda robot gardener which, once a day, can prune a single bamboo instantly. As you have measured the daily growth rate of each bamboo in the garden, now you need to program the robot gardener with an appropriate program to prune the bamboos to keep the best view.

The objective of the project is therefore to program the panda robot in order to optimize the cutting of bamboos, and to maintain the forest below a maximum threshold. Every day, the pattern is the same: the bamboos grow, the robot must choose which bamboo to cut (maybe there is none?) and cut the selected bamboo.
Several simple algorithms can be implemented: random, max height, max development (development=height+growth),...
To evaluate the different algorithms, several criteria may be taken into account. Criteria on the **size** of the bamboos (the maximum height of the remaining bamboos, the minimum height of the remaining bamboos (excluding the one cut :smirk:), the average height of the bamboos) and criteria on the **time** execution of the different methods.

### Formalization

![](https://codimd.math.cnrs.fr/uploads/upload_9aa63a8755091a23844a9806845d5690.png)


### Strategies
Two strategies naturally emerge: **Reduce-Max()** and **Reduce-Fastest(x)**:
- **Reduce-Max()** cuts the biggest bamboo of the moment. In this case, we notice experimentally that this makes it possible to maintain the max. of the forest below *2H*, where *H* is the growth of the whole forest (i.e., the sum of the daily growth rates).
- **Reduce-Fastest(x)** selects the bamboo that grows the fastest among those with a size greater than *xH*, where x>1. The choice of *x* that minimizes the max size of the forest is *x = 1 + 1/sqrt(5) ≈ 1.45*. This algorithm guarantees a maximum size of *2.62H* but does not cut bamboo at each turn (we save the panda robot's battery!).

Another critical aspect is the fact that your robot has limited capacities and memory! It is then important to quickly determine the next bamboo to be cut, in the most linear time (depending on the number of bamboos). This problem will be discussed later in the advanced part concerning **extensions**.

---

## Required work

### Level 0

In console mode, your program should be able at each step:
- program the daily growth of the bamboos,
- move the panda robot (several algorithm implementations),
- calculate bamboo height statistics, average, max height. reached, etc... and display them in the console.

Your program must also make it possible to configure the application (choice of the algorithm, the number of bamboos, etc.).

---

### Level 1

Level 0 elements can be taken over in SDL in order to display the stages of the chosen algorithm as well as the statistics curves. The interface can contain the following elements:
- **bamboo display**, which adapts to the max. bamboos
- **choice of strategy**, and choice of *x* for *Reduce-Fastest*
- **display of daily statistics curves**, as well as the max already reached. For the statistics, we can limit ourselves to the last T=100 measurements
- and any element that seems relevant to you to improve the display and the interaction with the user

*Help for display in sliding values*

* if T is the size of the array (T=100)
* as long as the table is not full (if i < T values) display them, from 0 to i
* if T values or more (i >= T), save the new values starting from the beginning, without erasing the following ones. For the ith value, simply save it in the i%T box, and display the values from (i+1)%T to (i+T)%T
* for example if T=100 and i=10, for the ith value, save in box 10, and display from 11 to 111 %100, which will give 100 values from 11 to 99 then 0 to 10. And the last value displayed is the one found in box 10.

Obviously, you have all the aesthetic freedom for your application, but this will not be the main point of evaluation. It must also, and above all, be as ergonomic as possible.

---

### Level 2

Therefore, many options can be considered. It's up to you to complete your application with one or more possible options:
- SDL buttons to **start/stop animation**
- add a **user interaction**. In this manual mode, the robot-panda is no longer programmed, it is the user who chooses which bamboo to cut, for example:
- with the arrows, to move the panda, and press a key to cut
- directly with a mouse click on the chosen bamboo
- be able to **save a configuration** (the number of bamboos and the growth chart, for example), and **reload it from a file**. Displaying the configuration in SDL
- we can also add an editing mode, either text (we enter the values in an SDL input area), or with *sliders*, or graphics (we display the bamboos after 1 day, and we modify their size on mouse help)

---

### Level 3

- **Computation time** criteria. In addition to the statistics curves on the size of the bamboos, it is interesting to evaluate the performance of your algorithms. Add a curve showing the execution times at each step of your algorithms.
- **Additional constraints**. It is possible to add constraints for the robot:
- the movement of the robot is **limited to a few squares** around the current position (this requires anticipating a more distant movement so as not to remain localized around a position). Remember to define a starting position,
- the robot being powered, it may need to return to its starting point to **recharge** the batteries:
- regularly, every 7 days for example
- or depending on the distance traveled (we can assume that the bamboos are regularly spaced. Also remember to keep the previous position of the panda to calculate the distance traveled each day.)
- despite a powerful algorithm, the forest still spoils your view, you buy a **second robot**...

---

### Level +
In extension, we can also:
- work in **2 dimensions**. In this case, the bamboos will be placed on a grid of *L* rows and *C* columns.
- resume the application to use 2-dimensional arrays
- adapt SDL display
- add specific constraints, for example the panda can only move in rows or columns for its daily movement
- implement **more efficient** algorithms...

Every day, the robot panda must examine all the bamboos. The 2 algorithms `Reduce-Max()` and `Reduce-Fastest(x)` therefore have a linear complexity in `O(n)`, where `n` is the number of bamboos.

It is of course possible to improve this by optimizing the search for the bamboo to be cut. For example, if we know that a part of the forest grows more slowly, we can test it less often. But there are many other possibilities!

This article [(Cutting Bamboo down to Size, 2021)](https://drops.dagstuhl.de/opus/volltexte/2020/12766/pdf/LIPIcs-FUN-2021-5.pdf) offers some of them, which however, require tree data structures, which you have not yet studied in algorithms...

And if we consider that the robot-panda has a limited autonomy, we can also look for algorithms that minimize the number of bamboos it cuts, or that minimize its movements.

---

## Analogy with computer science

A parallel can be made between this problem of the size of bamboos and the management of queues.
A waiting system is made up of a service area (comprising one or more servers) and a waiting area in which a possible queue is formed.
The literature distinguishes:
- open systems, where the queue (the number of pending entries) is of infinite length, and
- closed systems, where the queue is of finite size.

Several methods can then be highlighted for the management of these queues.

Given a queue, several characteristics are identifiable in order to measure the effectiveness of the chosen method:
- the average length of the queue,
- the average length of service
- the average rate of server inactivity,
- the average waiting time in a queue
- the total average service time in the system (or "residence time" in computing).

---

## Hardware and software needed
A PC, Visual Studio C++ (or any other equivalent IDE) for C++ development, the SDL graphics library, Git.

For a VSC++ project, you must install the **SDL nuggets**, but they will not be integrated into the Git repository.
Before doing the *push*, make sure that the **.gitignore** file is correctly configured, so as not to put the entire *.vs* folder on the Git repository! (Menu ```Git>Settings>General>.gitignore```)

When you **clone** a project from a Git repository on your workstation:
- for configuration in VSC++, often a simple click on the *.sln* file is enough
- you must of course also install the SDL *nuggets* for your local project

This only needs to be done once.

---

### Distance...
Git, jitsi, discord, etc.

---

## Groups {#planning_groups}
You will work in groups of 3 or 4:
* groups are determined **randomly**
* in the same TD group
* you must choose a **“project manager”** whose unofficial role will be
     * to distribute the tasks of each
     * to integrate the different codes
     * to evaluate the progress of the group
