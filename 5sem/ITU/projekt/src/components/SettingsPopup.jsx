import { useEffect, useState } from "react";
import AnimatedContent from "./AnimatedContent.jsx";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faO, faXmark, faPlus, faMinus, faExclamation} from '@fortawesome/free-solid-svg-icons';
import { api } from "./ApiInstance.ts";


function SettingsPopup({ displayPopup, currentColorTheme, winner, setColorTheme, colorThemes, player, modeValue, sizeValue, setSizeValue, setModeValue }) {

    const [btnHovered, setBtnHovered] = useState(null);

    const [sizeSettingHovered, setSizeSettingHovered] = useState(false);
    const [modeSettingHovered, setModeSettingHovered] = useState(false);

    const [sizePlusHovered, setSizePlusHovered] = useState(false);
    const [sizeMinusHovered, setSizeMinusHovered] = useState(false);

    const [modePlusHovered, setModePlusHovered] = useState(false);
    const [modeMinusHovered, setModeMinusHovered] = useState(false);

    useEffect(() => {
        if (sizeValue > 20) {
            setSizeValue(20);
            alert("Rozmery veľkosti hernej plochy môžu byť maximálne 20x20.");
            api.size = 20;
        }
        if (sizeValue < 3) {
            setSizeValue(3);
            alert("Rozmery hernej plochy môžu byť minimálne 3x3.");
            api.size = 3;
        }
        if (modeValue > sizeValue) {
            setModeValue(sizeValue);
            alert("Počet výherných políčok môže byť maximálne hodnota rozmeru hernej plochy.");
            api.squares_to_win = sizeValue;
        }
        if (modeValue < 3) {
            setModeValue(3);
            alert("Počet výherných políčok môže byť minimalne 3.");
            api.squares_to_win = 3;
        }

        if (sizeValue <= 20 && sizeValue >= 3 && modeValue >= 3 && modeValue <= sizeValue) {
            setModeValue(modeValue);
            setSizeValue(sizeValue);
            api.squares_to_win = modeValue;
            api.size = sizeValue;            
        }
        api.gameReset();       
         
        // eslint-disable-next-line
    }, [sizeSettingHovered, modeSettingHovered, btnHovered]);

    function handleSizePlus() {
        if (sizeValue >= 20) {
            setSizeValue(20);
            alert("Rozmery veľkosti hernej plochy môžu byť maximálne 20x20.");
            api.size = sizeValue;
        } 
        else {
            setSizeValue(sizeValue + 1);
            api.size = sizeValue + 1;
        }
        api.gameReset();

    }

    function handleSizeMinus() {
        if (sizeValue <= 3) {
            setSizeValue(3);
            alert("Rozmery hernej plochy môžu byť minimálne 3x3.");
            api.size = sizeValue;
        } else {
            setSizeValue(sizeValue - 1);
            api.size = sizeValue - 1;
        }
        api.gameReset();
    }


    function handleModePlus() {
        if (modeValue >= sizeValue) {
            setModeValue(sizeValue);
            api.squares_to_win = sizeValue; 
            alert("Počet výherných políčok môže byť maximálne hodnota rozmeru hernej plochy.")
        } else {
            setModeValue(modeValue + 1);
            api.squares_to_win = modeValue + 1; 
        }    
        api.gameReset();
    }

    function handleModeMinus() {
        if (modeValue <= 3) {
            setModeValue(3);
            alert("Počet výherných políčok môže byť minimálne 3.");
            api.squares_to_win = 3;
        } else {
            setModeValue(modeValue - 1);
            api.squares_to_win = modeValue - 1;
        }
        api.gameReset();
    }


    return (
        <>
            <div style={{
                position: 'absolute',
                height: '100%',
                width: '100%',
                backdropFilter: 'blur(3px)',
                zIndex: '3',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                flexDirection: 'column',
                rowGap: '1%'

            }}>
                <AnimatedContent
                    distance={500}
                    direction="vertical"
                    reverse={true}
                    duration={0.5}
                    ease="power.out"
                    initialOpacity={0}
                    animateOpacity
                    scale={1}
                    threshold={0.1}
                    delay={0}
                >
                    <div className="settings-change-notice"
                        style={{
                            position: 'relative',
                            border: '3px solid black',
                            borderRadius: '10px',
                            height: '100%',
                            width: '600px',
                            zIndex: '5',
                            backgroundColor: "white",
                            display: 'flex',
                            alignItems: 'center',
                            justifyContent: 'space-evenly',
                            textAlign: 'center',
                            paddingLeft: '20px',
                            paddingRight: '20px',
                            fontWeight: '500',
                            boxShadow: '0 -20px 180px black'
                        }}

                    >
                        <FontAwesomeIcon icon={faExclamation} style={{scale: '3', color: 'crimson'}} />   
                        <p>Zmeny veľkosti plochy alebo počtu výherných políčok <br/><b style={{color: 'crimson'}}>vymažú aktuálny herný progres!</b></p>
                        <FontAwesomeIcon icon={faExclamation} style={{scale: '3', color: 'crimson'}} />
                    </div>
                </AnimatedContent>              
                <AnimatedContent
                    distance={0}
                    direction="vertical"
                    reverse={true}
                    duration={0.5}
                    ease="power.out"
                    initialOpacity={1}
                    animateOpacity
                    scale={0}
                    threshold={0.1}
                    delay={0}
                >
                    <div className="settings-window"
                        style={{
                            height: '400px',
                            width: '600px',
                            border: '3px solid white',
                            borderRadius: '10px',
                            boxShadow: '0 100px 180px black',
                            backgroundColor: (winner === "tie") ? currentColorTheme.color2_tie : (player === "X") ? currentColorTheme.color2_x : currentColorTheme.color2_o,
                            zIndex: '4',
                            display: 'grid',
                            gridTemplateColumns: '5fr 5fr 5fr 1fr',
                            gridTemplateRows: '1fr 4fr 2fr 4fr 1fr 4fr',
                            padding: '20px',

                        }}
                    >
                        <div className="close-btn" 
                            style={{
                                    position: 'absolute',
                                    height: '50px',
                                    width: '50px',
                                    top: '5px',
                                    right: '5px',
                                    borderBottomLeftRadius: '15px',
                                    backgroundColor: 'transparent',
                                    scale: (btnHovered === 'close-btn') ? '1.8' : '1.3',
                                    color: 'white',
                                    display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    flexDirection: 'column',
                                    cursor: 'pointer',
                                    transition: 'scale 200ms ease, font-size 100ms ease',
                                }}
                            onMouseEnter={() => setBtnHovered('close-btn')}
                            onMouseLeave={() => setBtnHovered(null)} 
                            onClick={() => displayPopup(false)}
                        >
                            <FontAwesomeIcon icon={faXmark}/>


                        </div>
                        <div className="size-text" style={{
                            gridRow: '1',
                            gridColumn: 'span 2',
                            color: 'white',
                            display: 'flex',
                            alignItems: 'center',
                            justifyContent: 'start',
                        }}>
                            <h3 style={{textDecoration: ''}}> Nastavenie veľkosti plochy: </h3>
                        </div>
                        <div className="board-size-setting"
                            style={{
                                border: '2px solid transparent',
                                gridRow: '2',
                                gridColumn: 'span 4',
                                display: 'flex',
                                flexDirection: 'row',
                                alignItems: 'center',
                                justifyContent: 'space-evenly',
                                scale: sizeSettingHovered ? '1.02' : '1',
                                transition: 'scale 200ms ease',
                            }}
                            onMouseEnter={() => setSizeSettingHovered(true)}
                            onMouseLeave={() => setSizeSettingHovered(false)}
                        >
                            <div className="size-less"
                                style={{
                                    border:'2px solid white',
                                    height: '100%',
                                    width: '25%',
                                    cursor: 'pointer',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center',
                                    borderRadius: '10px',
                                    backgroundColor: sizeMinusHovered ? 'white' : 'transparent',
                                    transition: 'all 200ms ease',
                                    color: sizeMinusHovered ? 'black' : 'white'
                                }}
                                onMouseEnter={() => setSizeMinusHovered(true)}
                                onMouseLeave={() => setSizeMinusHovered(false)}
                                onClick={handleSizeMinus}
                                
                            >
                                <FontAwesomeIcon icon={faMinus} style={{scale: sizeMinusHovered ? '3' : '2.5', transition: 'scale 200ms ease' }} />
                            </div>
                            <input type="number" min="3" max="20" defaultValue={sizeValue} value={sizeValue} 
                            onChange={(event) => setSizeValue(event.target.value)} 
                            className="size-input"
                                style={{
                                    border: '2px solid white',
                                    borderRadius: '10px',
                                    height: '100%',
                                    width: '50%',
                                    fontSize: '50px',
                                    fontWeight: '500',
                                    textAlign: 'center',
                                    backgroundColor: (winner === "tie") ? currentColorTheme.color1_tie : (player === "X") ? currentColorTheme.color1_x : currentColorTheme.color1_o,
                                    color: 'white',
                                    outline: 'none',
                                    MozAppearance: 'textfield',
                                    appearance: 'textfield',
                                    

                                }}
                            >
                            </input>
                            <div className="size-more"
                                style={{
                                    border:'2px solid white',
                                    height: '100%',
                                    width: '25%',
                                    cursor: 'pointer',
                                    borderRadius: '10px',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center',
                                    backgroundColor: sizePlusHovered ? 'white' : 'transparent',
                                    transition: 'all 200ms ease',
                                    color: sizePlusHovered ? 'black' : 'white'
                                }}
                                onMouseEnter={() => setSizePlusHovered(true)}
                                onMouseLeave={() => setSizePlusHovered(false)}
                                onClick={handleSizePlus}
                            >
                                <FontAwesomeIcon icon={faPlus} style={{scale: sizePlusHovered ? '3' : '2.5', transition: 'scale 200ms ease' }} />
                            </div>
                        </div>

                        <div className="mode-text" style={{
                            gridRow: '3',
                            gridColumn: 'span 2',
                            color: 'white',
                            display: 'flex',
                            alignItems: 'center',
                            justifyContent: 'start',
                        }}>
                            <h3 style={{textDecoration: ''}}> Nastavenie počtu výherných políčok: </h3>
                        </div>

                        <div className="game-mode-setting"
                            style={{
                                gridRow: '4',
                                gridColumn: 'span 4',
                                display: 'flex',
                                flexDirection: 'row',
                                alignItems: 'center',
                                justifyContent: 'space-evenly',
                                scale: modeSettingHovered ? '1.02' : '1',
                                transition: 'scale 200ms ease',
                            }}
                            onMouseEnter={() => setModeSettingHovered(true)}
                            onMouseLeave={() => setModeSettingHovered(false)}
                        >
                            <div className="mode-less"
                                style={{
                                    border:'2px solid white',
                                    height: '100%',
                                    width: '25%',
                                    cursor: 'pointer',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center',
                                    borderRadius: '10px',
                                    backgroundColor: modeMinusHovered ? 'white' : 'transparent',
                                    transition: 'all 200ms ease',
                                    color: modeMinusHovered ? 'black' : 'white'
                                }}
                                onMouseEnter={() => setModeMinusHovered(true)}
                                onMouseLeave={() => setModeMinusHovered(false)}
                                onClick={handleModeMinus}
                                
                            >
                                <FontAwesomeIcon icon={faMinus} style={{scale: modeMinusHovered ? '3' : '2.5', transition: 'scale 200ms ease' }} />
                            </div>
                            <input type="number" min="3" max={api.size} defaultValue={modeValue} value={modeValue} 
                            onChange={(event) => setModeValue(event.target.value)} 
                            className="size-input"
                                style={{
                                    border: '2px solid white',
                                    borderRadius: '10px',
                                    height: '100%',
                                    width: '50%',
                                    fontSize: '50px',
                                    fontWeight: '500',
                                    textAlign: 'center',
                                    backgroundColor: (winner === "tie") ? currentColorTheme.color1_tie : (player === "X") ? currentColorTheme.color1_x : currentColorTheme.color1_o,
                                    color: 'white',
                                    outline: 'none',
                                    MozAppearance: 'textfield',
                                    appearance: 'textfield',
                                    

                                }}
                            >
                            </input>
                            <div className="mode-more"
                                style={{
                                    border:'2px solid white',
                                    height: '100%',
                                    width: '25%',
                                    cursor: 'pointer',
                                    borderRadius: '10px',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center',
                                    backgroundColor: modePlusHovered ? 'white' : 'transparent',
                                    transition: 'all 200ms ease',
                                    color: modePlusHovered ? 'black' : 'white'
                                }}
                                onMouseEnter={() => setModePlusHovered(true)}
                                onMouseLeave={() => setModePlusHovered(false)}
                                onClick={handleModePlus}
                            >
                                <FontAwesomeIcon icon={faPlus} style={{scale: modePlusHovered ? '3' : '2.5', transition: 'scale 200ms ease' }} />
                            </div>
                        </div>
                        <div className="theme-text" style={{
                            gridRow: '5',
                            gridColumn: 'span 2',
                            color: 'white',
                            display: 'flex',
                            alignItems: 'center',
                            justifyContent: 'start',
                        }}>
                            <h3 style={{textDecoration: ''}}> Nastavenie témy: </h3>
                        </div>
                        <div className="theme-options" 
                            style={{
                                    gridRow: '6',
                                    gridColumn: 'span 4',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center',
                                    gap: '5%',
                                }}
                            > 
                            <div className="color-theme1"
                                style={{
                                    height: '90%',
                                    width: '40%',
                                    scale: (JSON.stringify(currentColorTheme) === JSON.stringify(colorThemes[0])) ? '1.2' : '1',
                                    border: (JSON.stringify(currentColorTheme) === JSON.stringify(colorThemes[0])) ? '5px double white' : '2px solid white',
                                    borderRadius: '10px',
                                    cursor: 'pointer',
                                    transition: 'all 200ms ease',
                                    boxShadow: (btnHovered === 'color-theme1') ? '0 0 30px white' : JSON.stringify(currentColorTheme) === JSON.stringify(colorThemes[0]) ? '0 0 20px white' : '',
                                    display: 'flex', alignItems: 'center', justifyContent: 'space-between', flexDirection: 'row'
                                }}
                                onMouseEnter={() => setBtnHovered('color-theme1')}
                                onMouseLeave={() => setBtnHovered(null)}
                                onClick={() => setColorTheme(colorThemes[0])}
                            >   
                                <div style={{
                                    backgroundColor: colorThemes[0].color1_x,
                                    borderTopLeftRadius: '5px',
                                    borderBottomLeftRadius: '5px',
                                    width: '50%', 
                                    height: '100%', 
                                    display: 'flex', alignItems: 'center', justifyContent: 'center',
                                }} 
                                >
                                    <div style={{
                                        border: '1px solid white',
                                        borderRadius: '5px',
                                        width: '60%',
                                        height: '80%',
                                        backgroundColor: colorThemes[0].color2_x,
                                        color: 'white',
                                        fontSize: '30px',
                                        display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    }}>
                                        <FontAwesomeIcon icon={faXmark}/>
                                    </div>
                                </div>

                                <div style={{
                                    backgroundColor: colorThemes[0].color1_o,
                                    borderTopRightRadius: '5px',
                                    borderBottomRightRadius: '5px',
                                    width: '50%',
                                    height: '100%',
                                    display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    }}
                                >
                                    <div style={{
                                        border: '1px solid white',
                                        borderRadius: '5px',
                                        width: '60%',
                                        height: '80%',
                                        backgroundColor: colorThemes[0].color2_o,
                                        color: 'white',
                                        fontSize: '30px',
                                        display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    }}>
                                        <FontAwesomeIcon icon={faO}/>
                                    </div>
                                </div>
                            </div>


                            <div className="color-theme2"
                                style={{
                                    height: '90%',
                                    width: '40%',
                                    scale: JSON.stringify(currentColorTheme) === JSON.stringify(colorThemes[1]) ? '1.2' : '1',
                                    backgroundColor: 'transparent',
                                    border: JSON.stringify(currentColorTheme) === JSON.stringify(colorThemes[1]) ? '7px double white' : '2px solid white',
                                    borderRadius: '10px',
                                    color: 'white',
                                    fontSize: '20px',
                                    fontWeight: '600',
                                    cursor: 'pointer',
                                    transition: 'all 200ms ease',
                                    boxShadow: (btnHovered === 'color-theme2') ? '0 0 30px white' : JSON.stringify(currentColorTheme) === JSON.stringify(colorThemes[1]) ? '0 0 20px white' : '',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center',

                                }}
                                onMouseEnter={() => setBtnHovered('color-theme2')}
                                onMouseLeave={() => setBtnHovered(null)}
                                onClick={() => setColorTheme(colorThemes[1])}
                            >
                                <div style={{
                                    backgroundColor: colorThemes[1].color1_x,
                                    borderTopLeftRadius: '5px',
                                    borderBottomLeftRadius: '5px',
                                    width: '50%', 
                                    height: '100%', 
                                    display: 'flex', alignItems: 'center', justifyContent: 'center',
                                }} 
                                >
                                    <div style={{
                                        border: '1px solid white',
                                        borderRadius: '5px',
                                        width: '60%',
                                        height: '80%',
                                        backgroundColor: colorThemes[1].color2_x,
                                        color: 'white',
                                        fontSize: '30px',
                                        display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    }}>
                                        <FontAwesomeIcon icon={faXmark}/>
                                    </div>
                                </div>

                                <div style={{
                                    backgroundColor: colorThemes[1].color1_o,
                                    borderTopRightRadius: '5px',
                                    borderBottomRightRadius: '5px',
                                    width: '50%',
                                    height: '100%',
                                    display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    }}
                                >
                                    <div style={{
                                        border: '1px solid white',
                                        borderRadius: '5px',
                                        width: '60%',
                                        height: '80%',
                                        backgroundColor: colorThemes[1].color2_o,
                                        color: 'white',
                                        fontSize: '30px',
                                        display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    }}>
                                        <FontAwesomeIcon icon={faO}/>
                                    </div>
                                </div>
                            </div>

                            <div className="color-theme3"
                                style={{
                                    height: '90%',
                                    width: '40%',
                                    scale: (JSON.stringify(currentColorTheme) === JSON.stringify(colorThemes[2])) ? '1.2' : '1',
                                    border: (JSON.stringify(currentColorTheme) === JSON.stringify(colorThemes[2])) ? '5px double white' : '2px solid white',
                                    borderRadius: '10px',
                                    cursor: 'pointer',
                                    transition: 'all 200ms ease',
                                    boxShadow: (btnHovered === 'color-theme3') ? '0 0 30px white' : JSON.stringify(currentColorTheme) === JSON.stringify(colorThemes[2]) ? '0 0 20px white' : '',
                                    display: 'flex', alignItems: 'center', justifyContent: 'space-between', flexDirection: 'row'
                                }}
                                onMouseEnter={() => setBtnHovered('color-theme3')}
                                onMouseLeave={() => setBtnHovered(null)}
                                onClick={() => setColorTheme(colorThemes[2])}
                            >   
                                <div style={{
                                    backgroundColor: colorThemes[2].color1_x,
                                    borderTopLeftRadius: '5px',
                                    borderBottomLeftRadius: '5px',
                                    width: '50%', 
                                    height: '100%', 
                                    display: 'flex', alignItems: 'center', justifyContent: 'center',
                                }} 
                                >
                                    <div style={{
                                        border: '1px solid white',
                                        borderRadius: '5px',
                                        width: '60%',
                                        height: '80%',
                                        backgroundColor: colorThemes[2].color2_x,
                                        color: 'white',
                                        fontSize: '30px',
                                        display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    }}>
                                        <FontAwesomeIcon icon={faXmark}/>
                                    </div>
                                </div>

                                <div style={{
                                    backgroundColor: colorThemes[2].color1_o,
                                    borderTopRightRadius: '5px',
                                    borderBottomRightRadius: '5px',
                                    width: '50%',
                                    height: '100%',
                                    display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    }}
                                >
                                    <div style={{
                                        border: '1px solid white',
                                        borderRadius: '5px',
                                        width: '60%',
                                        height: '80%',
                                        backgroundColor: colorThemes[2].color2_o,
                                        color: 'white',
                                        fontSize: '30px',
                                        display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    }}>
                                        <FontAwesomeIcon icon={faO}/>
                                    </div>
                                </div>
                            </div>


                        </div>
                    </div>
                </AnimatedContent>
            </div>
        </>
    );
}

export default SettingsPopup;