### Data Structure
[
    {
        "name": "Splitscreen",
        "options": [
            {
                "type": "number",
                "label": null,
                "value": 0
            },
            {
                "type": "bool",
                "label": "Use K/M for Player 1",
                "value": true
            },
            {
                "type": "bool",
                "label": "Use Player 1 Profile",
                "value": false
            }
            {
                "type": "bool",
                "label": "Override Profile",
                "value": false
            }
        ]
    },
    {
        "name": "Player 1",
        "options": [
            {
                "type": "picker",
                "label": null,
                "value": [
                    {
                        "type": "pickerOption",
                        "label": "Controller 1",
                        "value": 0
                    },
                    {
                        "type": "pickerOption",
                        "label": "Controller 2",
                        "value": 1
                    },
                    {
                        "type": "pickerOption",
                        "label": "Controller 3",
                        "value": 2
                    },
                    {
                        "type": "pickerOption",
                        "label": "Controller 4",
                        "value": 3
                    }
                ]
            },
            {
                "type": "picker",
                "label": "Primary"
                "value: [
                    {
                        "type": "pickerOption",
                        "label": "Red",
                        "value": 0
                    },
                    {
                        "type": "pickerOption",
                        "label": "Blue",
                        "value": 1
                    },
                    {
                        "type": "pickerOption",
                        "label": "Green",
                        "value": 2
                    },
                ]
            },
            {
                "type": "picker",
                "label": "Secondary"
                "value: [
                    {
                        "type": "pickerOption",
                        "label": "Red",
                        "value": 0
                    },
                    {
                        "type": "pickerOption",
                        "label": "Blue",
                        "value": 1
                    },
                    {
                        "type": "pickerOption",
                        "label": "Green",
                        "value": 2
                    },
                ]
            },
            {
                "type": "picker",
                "label": "Tertiary"
                "value: [
                    {
                        "type": "pickerOption",
                        "label": "Red",
                        "value": 0
                    },
                    {
                        "type": "pickerOption",
                        "label": "Blue",
                        "value": 1
                    },
                    {
                        "type": "pickerOption",
                        "label": "Green",
                        "value": 2
                    },
                ]
            },
        ]
    }
]

### Project Structure
xbox360menu/
│
├── CMakeLists.txt
├── assets/
│   ├── fonts/
│   ├── textures/
│   └── sounds/
│
├── src/
│   ├── main.cpp
│   │
│   ├── core/
│   │   ├── Application.hpp
│   │   ├── Application.cpp
│   │   ├── Time.hpp
│   │   └── Time.cpp
│   │
│   ├── platform/
│   │   ├── Window.hpp
│   │   ├── Window.cpp
│   │   ├── Renderer.hpp
│   │   ├── Renderer.cpp
│   │   ├── Input.hpp
│   │   └── Input.cpp
│   │
│   ├── ui/
│   │   ├── UIElement.hpp
│   │   ├── UIButton.hpp
│   │   ├── UILabel.hpp
│   │   ├── VerticalMenu.hpp
│   │   ├── Animation.hpp
│   │   └── Easing.hpp
│   │
│   ├── states/
│   │   ├── State.hpp
│   │   ├── PauseMenuState.hpp
│   │   └── PauseMenuState.cpp
│   │
│   └── utils/
│       └── Math.hpp
│
└── third_party/

### Navigation
Using left/right navigates between the items in the main array.  Using up/down navigates through the options in the active item in the main array.  Enter selects and esc navigates backward.  Picker types are sub pages that display only a list of options.
