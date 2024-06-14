
GitHub: https://github.com/SDuncan5/csc471-final

Introduction
	My final project was initially going to have a randomly spawned chest in the world and the player was going to search for it in a height-mapped world, but I wasn’t able to implement all of those technologies in time unfortunately. So, my final was focused around making the player character (skeleton pirate) move around the world with user inputs and creating a 3rd person camera on an island scene.

Inputs:
W to move forwards
S to move backwards
A to turn left
D to turn right
Left shift to sprint when moving forwards

Hold M1 and move mouse back and forth to circle around player

Required Components:

Complex Environment
* includes a sky sphere, palm trees littered around the ground plane, a treasure chest, a player, and a dog (that was originally going to follow the character)
* Additionally, there’s a pirate ship w/ the hierarchical model in the background

Lighting
* Has ambient, diffuse, specular lighting for all regular models. No specular lighting for textured objects
* Moved the light source far away and increased the ambient lighting (x, y, z = 200) to appear more like the Sun

Hierarchical Modeling
* Included the original waving model on the pirate ship

Game Camera
* Overhauled the game camera to become a 3rd person camera
* Move around the world by moving the player

Enhanced Technology
* Created functionality to make the user control the player
* Overhauled the camera to make it follow the player.

Extras
The treasure chest model randomly spawns on the “island” every time the game is loaded up.
I was initially going to make a more advanced 3rd person camera (and spent a lot of time on it) where you could adjust the pitch, zoom in closer or further from the character, and also circle around the character. I was able to get the zoom working and was able to adjust the pitch, but the camera wasn’t properly following the character. So, I opted to have a more simple camera that worked properly.
In addition, the palm trees “randomly” spawn around the map so every time you open the game, they’ll be in different locations.

Images
https://youtu.be/7yZ2EhsxVAk

References
Player Movement: https://www.youtube.com/watch?v=d-kuzyCkjoQ&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=19&ab_channel=ThinMatrix
3D Camera: https://www.youtube.com/watch?v=PoxDDZmctnU&ab_channel=ThinMatrix
3D Camera OpenGL: https://www.youtube.com/watch?v=eJ6IQY02cdY&list=PL6xSOsbVA1eYSZTKBxnoXYboy7wc4yg-Z&index=42&ab_channel=SurajSharma




