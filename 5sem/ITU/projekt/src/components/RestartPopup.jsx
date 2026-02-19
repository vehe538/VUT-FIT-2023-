import { useState } from "react";
import AnimatedContent from "./AnimatedContent.jsx"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faRotate, faXmark} from '@fortawesome/free-solid-svg-icons'
import { api } from "./ApiInstance.ts";

function RestartPopup({ displayPopup, currentColorTheme, winner, player, setPlayer, setWinner, setMenuOpen}) {

    const [btnHovered, setBtnHovered] = useState(null);

    function handleYesClick() {
        api.gameReset();
        setPlayer("");
        setWinner(null);
        displayPopup(false);
        setMenuOpen(false);
    }

    function handleNoClick() {
        setMenuOpen(false);
        displayPopup(false)
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
                justifyContent: 'center'

            }}>
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
                    <div className="confirm-window"
                        style={{
                            height: '250px',
                            width: '500px',
                            border: '3px solid white',
                            borderRadius: '10px',
                            boxShadow: '0 0 180px black',
                            backgroundColor: (winner === "tie") ? currentColorTheme.color2_tie : (player === "X") ? currentColorTheme.color2_x : currentColorTheme.color2_o,
                            zIndex: '4',
                            display: 'grid',
                            gridTemplateColumns: '1fr 8fr 1fr',
                            gridTemplateRows: '1fr 4fr 2fr',

                        }}
                    >
                        <div className="close-btn" 
                            style={{
                                    gridRow: '1', 
                                    gridColumn: '3',
                                    borderBottomLeftRadius: '15px',
                                    backgroundColor: 'transparent',//(btnHovered === 'close-btn') ? 'rgb(185, 7, 7)' : 'red',
                                    fontSize: (btnHovered === 'close-btn') ? '25px' : '18px',
                                    transition: 'font-size 100ms ease',
                                    color: 'white',
                                    display: 'flex', alignItems: 'center', justifyContent: 'center',
                                    cursor: 'pointer'
                                }}
                            onMouseEnter={() => setBtnHovered('close-btn')}
                            onMouseLeave={() => setBtnHovered(null)} 
                            onClick={() => displayPopup(false)}
                        >
                            <FontAwesomeIcon icon={faXmark}/>


                        </div>

                        <div className="text" style={{
                            gridRow: '2',
                            gridColumn: '2',
                            color: 'white',
                            textAlign: 'center',
                        }}>
                            <h1> Herný progres sa vymaže! </h1>
                            <h3> Naozaj chceš reštartovať hru? </h3>
                        </div>

                        <div className="options" 
                            style={{
                                    gridRow: '3',
                                    gridColumn: 'span 3',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center',
                                    gap: '5%',
                                    paddingBottom: '10px'
                                }}> 
                            <button className="options-no"
                                style={{
                                    height: '90%',
                                    width: '40%',
                                    scale: (btnHovered === 'options-no') ? '1.05' : '1',
                                    backgroundColor: (btnHovered === 'options-no') ? 'transparent' : 'transparent',
                                    border: '2px solid white',
                                    borderRadius: '15px',
                                    color: 'white',
                                    fontSize: '20px',
                                    fontWeight: '600',
                                    cursor: 'pointer',
                                    transition: 'all 200ms ease',
                                    boxShadow: (btnHovered === 'options-no') ? '0 0 20px white' : '',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center'
                                }}
                                onMouseEnter={() => setBtnHovered('options-no')}
                                onMouseLeave={() => setBtnHovered(null)}
                                onClick={handleNoClick}
                            >   
                                

                                Pokračovať v hre
                            </button>
                            <button className="options-yes"
                                style={{
                                    height: '90%',
                                    width: '40%',
                                    scale: (btnHovered === 'options-yes') ? '1.1' : '1',
                                    backgroundColor: (btnHovered === 'options-yes') ? 'rgb(32, 194, 0)' : 'transparent',
                                    border: '2px solid white',
                                    borderRadius: '15px',
                                    color: 'white',
                                    fontSize: '20px',
                                    fontWeight: '600',
                                    cursor: 'pointer',
                                    transition: 'all 200ms ease',
                                    boxShadow: (btnHovered === 'options-yes') ? '0 0 30px green' : '',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center',
                                    gap: (btnHovered === 'options-yes') ? '15px' : '10px'

                                }}
                                onMouseEnter={() => setBtnHovered('options-yes')}
                                onMouseLeave={() => setBtnHovered(null)}
                                onClick={handleYesClick}
                            >
                            <FontAwesomeIcon icon={faRotate} 
                                style={{
                                    scale: (btnHovered === 'options-yes') ? '1.2' : '1',
                                    transform: (btnHovered === 'options-yes') ? 'rotate(120deg)' : '',
                                    transition: 'scale 200ms ease, transform 300ms ease',
                                }}
                            />
                                Reštart hry
                            </button>
                        </div>
                    </div>
                </AnimatedContent>
            </div>
        </>
    );
}

export default RestartPopup;