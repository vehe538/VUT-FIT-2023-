import { useState } from "react";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faPlay, faGears, faCircleQuestion, faRightFromBracket, faRotate} from '@fortawesome/free-solid-svg-icons'

function MenuButton({buttonText, id, setMenuStatus, setWinner, setPlayer, colorTheme, setTheme, displayPopup}) {

    const [isHovered, setIsHovered] = useState(false);
    
    var buttonIcon = null;

    switch(id) {
        case "continue-button":
            buttonIcon = faPlay;
            break;
        case "settings-button":
            buttonIcon = faGears; 
            break;
        case "help-button":
            buttonIcon = faCircleQuestion;
            break;
        case "exit-button":
            buttonIcon = faRightFromBracket;
            break;
        case "restart-button":
            buttonIcon = faRotate;
            break;
        default:
            break;
    }

    function handleClick() {
        switch(id) {
            case "continue-button":
                setMenuStatus(false);
                break;

            default:
                displayPopup(true);
                break;
        }

    }

    return (
        <>
            <button 
                style={{
                    width: (id !== "exit-button") ? '100%' : '50%',
                    height: '50px', 
                    fontSize: '20px',
                    fontWeight: '600',
                    border: (id !== "exit-button") ? '0px solid black' : '3px solid white', 
                    borderRadius: (id !== "exit-button") ? '' : '15px',
                    borderTopRightRadius: (id === "continue-button") ? '15px':  (id !== "exit-button") ? '' : '15px', 
                    backgroundColor: isHovered ? 'white' : 'transparent',
                    color: isHovered ? 'black' : 'white',
                    cursor: 'pointer',
                    transition: 'all 200ms ease',
                    display: 'flex',
                    alignItems: 'center',
                    justifyContent: 'space-between',
                    flexDirection: (id === "exit-button") ? 'row-reverse' : '',
                    paddingRight: '50px',
                    paddingLeft: (id === "exit-button" && isHovered) ? '35px' : '50px',
                }}
                onMouseEnter={() => setIsHovered(true)}
                onMouseLeave={() => setIsHovered(false)}
                onClick={handleClick}
            >
                {buttonText}
                <FontAwesomeIcon icon={buttonIcon} 
                    style={{scale: (isHovered) ? '1.5' : '1',
                        transform: (id === "exit-button") ? 'rotate(180deg)' : '',
                            transition: 'scale 200ms ease',
                        }}
                />
            </button>

        
        </>
    )
}

export default MenuButton;