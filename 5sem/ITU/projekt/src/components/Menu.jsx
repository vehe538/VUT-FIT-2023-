import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faBars} from '@fortawesome/free-solid-svg-icons'
import { useState } from "react";
import MenuButton from "./MenuButton";
import ExitPopup from "./ExitPopup";
import RestartPopup from "./RestartPopup";
import HelpPopup from "./HelpPopup";
import SettingsPopup from "./SettingsPopup";

function Menu({ currentColorTheme, setColorTheme, colorThemes, winner, setWinner, setPlayer, player, mode, size, setModeValue, setSizeValue }) {

    const [menuOpen, setMenuOpen] = useState(false);
    const [buttonHovered, setButtonHovered] = useState(false);

    // popups
    const [exitPopup, displayExitPopup] = useState(false);
    const [restartPopup, displayRestartPopup] = useState(false);
    const [helpPopup, displayHelpPopup] = useState(false);
    const [settingsPopup, displaySettingsPopup] = useState(false)

    return(
        <>
            <div className="blurred-overlay"
                style={{
                    position: 'absolute',
                    height: '100%',
                    width: '100%',
                    border: '2px solid black',
                    alignItems: 'center',
                    justifyContent: 'flex-start',
                    display: menuOpen ? 'flex' : 'none',
                    backdropFilter: 'blur(10px)',
                    zIndex: '2',
                }}
            > 
            </div>

            <div className="menu" 
                style={{
                
                position: 'absolute',
                height: '600px',
                width: '500px',
                top: '10%',
                display: 'flex',
                justifyContent: 'space-between',
                alignItems: 'flex-start',
                flexDirection: 'row',
                left: menuOpen ? '0px' : buttonHovered ? '-400px' : '-450px',
                transition: 'all 300ms ease',
                zIndex: '3'

            }}>
                <div className="menu-open-options"
                    style={{
                        height: '600px',
                        width:' 420px',
                        border: '2px solid white',
                        borderLeft: '2px solid transparent',
                        borderBotoom: '2px solid white',
                        borderRight: menuOpen ? '2px solid white' : '',
                        borderTopRightRadius: menuOpen ? '15px' : '0px',
                        display: 'flex',
                        flexDirection: 'column',
                        alignItems: 'center',
                        justifyContent: 'flex-start',
                        backgroundColor: (winner === "tie") ? currentColorTheme.color2_tie : (player === "X") ? currentColorTheme.color2_x : currentColorTheme.color2_o,
                        boxShadow: '0 0 30px white',
                        transition: 'background-color 300ms ease',
                        zIndex: menuOpen ? '1' : '2',
                        borderBottomRightRadius: '15px',
                    }}
                >
                    <div style={{
                        width: '420px', 
                        height: '400px',
                        display: 'flex',
                        alignItems: 'center',
                        justifyContent: 'flex-start',
                        flexDirection: 'column',
                        }}> 
                        <MenuButton buttonText={"Pokračovať"} id={"continue-button"} setMenuStatus={setMenuOpen}/>
                        <MenuButton buttonText={"Reštart"} id={"restart-button"}     setMenuStatus={setMenuOpen} currentColorTheme={currentColorTheme} displayPopup={displayRestartPopup}  setWinner={setWinner} setPlayer={setPlayer}/>
                        <MenuButton buttonText={"Nastavenia"} id={"settings-button"} setMenuStatus={setMenuOpen} currentColorTheme={currentColorTheme} displayPopup={displaySettingsPopup} setColorTheme={setColorTheme} mode={mode}/>
                        <MenuButton buttonText={"Nápoveda"} id={"help-button"}       setMenuStatus={setMenuOpen} currentColorTheme={currentColorTheme} displayPopup={displayHelpPopup}/>
                        
                    </div>   
                    <div style={{
                        width: '420px', 
                        height: '400px',
                        display: 'flex',
                        alignItems: 'center',
                        justifyContent: 'flex-end',
                        flexDirection: 'column',
                        paddingBottom: '20px',
                        }}> 
                        <MenuButton buttonText={"Koniec"} id={"exit-button"} setMenuStatus={setMenuOpen} displayPopup={displayExitPopup}/>
                    </div>
                </div>
                <div className="menu-open-button"
                    style={{
                        height: '50px',
                        width: '100px',
                        display: 'flex',
                        opacity: menuOpen ? '0' : '1',
                        alignItems: 'center',
                        justifyContent: menuOpen ? 'flex-end' : 'flex-end',
                        paddingRight: menuOpen ? '25px' : '15px',
                        cursor: menuOpen ? '' : 'pointer',
                        backgroundColor: (winner === "tie") ? currentColorTheme.color2_tie : (player === "X") ? currentColorTheme.color2_x : currentColorTheme.color2_o,
                        transition: 'all 200ms ease',
                        border: '2px solid white',
                        boxShadow: buttonHovered ? '12px 0 30px white' : '5px 0 15px white',
                        borderTopRightRadius: '10px',
                        borderBottomRightRadius: '10px',
                        
                    }}
                    onClick={() => setMenuOpen(!menuOpen)}
                    onMouseEnter={() => setButtonHovered(true)}
                    onMouseLeave={() => setButtonHovered(false)}
                >
                    <FontAwesomeIcon icon={faBars} style={{ scale: '2.5', color: 'white'}}/>
                </div>
            </div>
            {
                (exitPopup)     ? <ExitPopup     displayPopup={displayExitPopup}     currentColorTheme={currentColorTheme} winner={winner} player={player} setPlayer={setPlayer} setMenuOpen={setMenuOpen}/> : ''
            }
            {
                (restartPopup)  ? <RestartPopup  displayPopup={displayRestartPopup}  currentColorTheme={currentColorTheme} winner={winner} player={player} setPlayer={setPlayer} setWinner={setWinner} setMenuOpen={setMenuOpen} /> : ''
            }
            {
                (helpPopup)     ? <HelpPopup     displayPopup={displayHelpPopup}     currentColorTheme={currentColorTheme} winner={winner} player={player} /> : ''
            }
            {
                (settingsPopup) ? <SettingsPopup displayPopup={displaySettingsPopup} currentColorTheme={currentColorTheme} winner={winner} setColorTheme={setColorTheme} colorThemes={colorThemes} player={player} modeValue={mode} sizeValue={size} setModeValue={setModeValue} setSizeValue={setSizeValue}  /> : ''
            }
        </>
    )
}

export default Menu;