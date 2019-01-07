# MazeChallenge_C
## Info
WIP solution for Amazon Maze Challenge from (http://c.adccpt.com/#/challenges/2c9f9f0c545bb87e015494c00b11003c) converted to Python.

Currently a bruteforce solution working up till 5x5 grid size due to CPU time constraints.

Idea for this solution is to use opencv and a floodfill to find if a path exists from the top of the maze till the bottom.

## Results
Failed Paths:

![alt text](https://raw.githubusercontent.com/rokaN8/MazeChallenge_Python/master/readme_images/failed/image0.jpg)
![alt text](https://raw.githubusercontent.com/rokaN8/MazeChallenge_Python/master/readme_images/failed/image355.jpg)
![alt text](https://raw.githubusercontent.com/rokaN8/MazeChallenge_Python/master/readme_images/failed/image90.jpg)
![alt text](https://raw.githubusercontent.com/rokaN8/MazeChallenge_Python/master/readme_images/failed/image91.jpg)

Successful Paths:

![alt text](https://raw.githubusercontent.com/rokaN8/MazeChallenge_Python/master/readme_images/success/image227.jpg)
![alt text](https://raw.githubusercontent.com/rokaN8/MazeChallenge_Python/master/readme_images/success/image231.jpg)
![alt text](https://raw.githubusercontent.com/rokaN8/MazeChallenge_Python/master/readme_images/success/image241.jpg)
![alt text](https://raw.githubusercontent.com/rokaN8/MazeChallenge_Python/master/readme_images/success/image59.jpg)


Solutions:

3x3 Grid: 78 (different from their solution which states 74) in 0.033s

4x4 Grid: 10814 in 0.622s

5x5 Grid: 5695680 in 317.038s (5m17.038s)

6x6 Grid: .... 
