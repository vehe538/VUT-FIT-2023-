import { useState } from "react";
import AnimatedContent from "./AnimatedContent.jsx"

import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faXmark} from '@fortawesome/free-solid-svg-icons'


function HelpPopup({ displayPopup, currentColorTheme, winner, player }) {

    const [btnHovered, setBtnHovered] = useState(null);

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
                    <div className="rules-window"
                        style={{
                            height: '750px',
                            width: '500px',
                            border: '3px solid white',
                            borderRadius: '10px',
                            boxShadow: '0 0 180px black',
                            backgroundColor: (winner === "tie") ? currentColorTheme.color2_tie : (player === "X") ? currentColorTheme.color2_x : currentColorTheme.color2_o,
                            zIndex: '4',
                            display: 'grid',
                            gridTemplateColumns: '1fr 8fr 1fr',
                            gridTemplateRows: '1fr 15fr 2fr',

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
                            border: '2px solid transparent',
                            gridRow: '2',
                            gridColumn: '2',
                            color: 'white',
                            textAlign: 'center',
                            padding: '5%',
                            fontWeight: '400',
                            lineHeight: '25px',
                            overflowY: 'auto',
                            maskImage: 'linear-gradient(to bottom, transparent, black 5%, black 95%, transparent)'
                        }}>
                            <h2> Pravidlá hry: </h2>
                            <p style={{fontSize: '19px', }}> 
                                Cieľom hry je vytvoriť radu troch rovnakých symbolov - buď <b> krížikov (X) </b> alebo <b> koliesok (O) </b> 
                                v riadku, stĺpci alebo diagonále.
                            </p>
                            <ul style={{textAlign: 'start', fontWeight: '450', fontSize: '18px', marginLeft: '-20px'}}>
                                <li><span style={{textDecoration: 'underline'}}>Herné pole:</span>
                                    <ul style={{fontSize: '16px', fontWeight: '400'}}>
                                        <li>Hrá sa na štvorcovej mriežke s rozmermi <b>3x3</b>.</li>
                                        <li>Každé políčko môže byť prázdne, obsahovať <b>X</b> alebo <b>O</b>.</li>
   
                                    </ul>
                                </li>
                                <br/>
                                <li><span style={{textDecoration: 'underline'}}>Hráči:</span>
                                    <ul style={{fontSize: '16px', fontWeight: '400'}}>
                                        <li>Hrajú <b>dvaja hráči</b>.</li>
                                        <li>Jeden hráč používa symbol <b>X</b>, druhý <b>O</b>.</li>
                                        <li>Hráči sa <b>striedajú v ťahoch</b>.</li>
                                    </ul>
                                </li>
                                <br/>
                                <li><span style={{textDecoration: 'underline'}}>Priebeh hry:</span>
                                    <ol style={{fontSize: '16px', fontWeight: '400'}}>
                                        <li>Na začiatku je celé pole prázdne.</li>
                                        <li>Hráč, ktorý má <b>X</b>, začína hru.</li>
                                        <li>ráči sa <b>striedajú po jednom ťahu</b>, pričom každý ťah spočíva v umiestnení svojho symbolu na jedno voľné políčko.</li>
                                        <li>Po každom ťahu sa kontroluje, či niektorý hráč <b>nevytvoril radu troch rovnakých symbolov</b>:
                                            <ul>
                                                <li><b>vodorovne (riadok)</b></li>
                                                <li><b>zvisle (stĺpec)</b></li>
                                                <li><b>uhlopriečne (diagonála)</b></li>
                                            </ul>
                                        </li>
                                    </ol>
                                </li>
                                <br/>
                                <li><span style={{textDecoration: 'underline'}}>Koniec hry:</span>
                                    <ul style={{fontSize: '16px', fontWeight: '400'}}>
                                        <li>Hra končí, keď:
                                            <ul>
                                                <li>niektorý hráč <b>vytvorí trojicu</b> rovnakých symbolov → <b>vyhráva</b>,</li>
                                                <li>alebo sa <b>zaplnia všetky políčka</b> a nikto nevytvoril trojicu → <b>remíza</b>.</li>
                                            </ul>
                                        </li>
                                    </ul>
                                </li>
                            </ul>

                        </div>

                        <div className="options" 
                            style={{
                                    gridRow: '3',
                                    gridColumn: 'span 3',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center',
                                    paddingBottom: '10px',
                                    paddingTop: '10px'    
                            }}>   
                                
                            <button className="options-yes"
                                style={{
                                    height: '90%',
                                    width: '40%',
                                    scale: (btnHovered === 'options-yes') ? '1.05' : '1',
                                    backgroundColor: 'transparent',
                                    border: '2px solid white',
                                    borderRadius: '15px',
                                    color: 'white',
                                    fontSize: '20px',
                                    fontWeight: '600',
                                    cursor: 'pointer',
                                    transition: 'all 200ms ease',
                                    boxShadow: (btnHovered === 'options-yes') ? '0 0 20px white' : '',
                                    display: 'flex',
                                    alignItems: 'center',
                                    justifyContent: 'center',
                                    gap: (btnHovered === 'options-yes') ? '15px' : '10px'

                                }}
                                onMouseEnter={() => setBtnHovered('options-yes')}
                                onMouseLeave={() => setBtnHovered(null)}
                                onClick={() => displayPopup(false)}
                            >
                                OK
                            </button>
                        </div>
                    </div>
                </AnimatedContent>
            </div>
        </>
    );
}

export default HelpPopup;