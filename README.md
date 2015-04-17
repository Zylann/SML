SML 
=====

Simple Markup Language (sorry I'm lazy)

SML is a home-made text format designed for easy human read/write of data files.
It's a superset of JSON, inspired by QML, with a few handy features:

- Quotes are optional
- Commas after values are optional
- Values can be explicitely typed by prepending their name
- '(' and ')' are alternate characters for '[' and ']'
- C-style comments
- Optional #format header to identify the contents of a file

The format is neither aimed at replacing JSON nor being more efficient,
but just to give a nicer syntax to write by hand, a way to easily embed types, 
and also because it was fun to write a parser.

Example
-------

SML:

	#format SML1
	Player {
		position: (1,2,3)
		rotation: (0,0,0,1) // Or can be Quaternion(0,0,0,1)
		color: "#ffaa44"
		children: [
			Gun // A node of type Gun with no value
			Sprite {
				material: {
					shader: "emissive"
					texture: "C:/game/assets/player.png"
					// Or we could write texture: fullpath("...")
				}
			}
		]
	}

JSON equivalent:
	
	{
		"type":"Player",
		"position":[1,2,3],
		"rotation:[0,0,0,1],
		"color":"#ffaa44",
		"children":[
			{
				"type":"Gun"
			},
			{
				"type":"Sprite",
				"material":{
					"shader":"emissive",
					"texture":"C:/game/assets/player.png"
				}
			}
		]
	}

Notes: the texture property carries more info than its JSON version
	
Minified versions:

SML:
	Player{position:(1,2,3),rotation:(0,0,0,1),color:"#ffaa44",children:[Gun,Sprite{material:{shader:"emissive",texture:"C:/game/assets/player.png"}}]}

JSON:
	{"type":"Player","position":[1,2,3],"rotation":[0,0,0,1],"color":"#ffaa44","children":[{"type":"Gun"},{"type":"Sprite","material":{"shader":"emissive","u_Texture":"C:/game/assets/player.png"}}]}


TODO
-------

- Errors reporting with line and column number
- Better comments support
- #format tag support
- General optimization
- Option to include comments into parsed data
- Unicode
- TypedObjects without value
