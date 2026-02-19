import { useState, useEffect } from "react";
//import { useNavigate } from "react-router-dom";

import { api } from "./ApiInstance.ts";

import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faXmark, faO, faUserGroup} from '@fortawesome/free-solid-svg-icons'
import AnimatedContent from "./AnimatedContent.jsx"
import Menu from "./Menu.jsx";
import PlayboardBlocker from "./PlayboardBlocker.jsx";


function TicTacToe2() {

    const [player, setPlayer] = useState(null);
    const [winner, setWinner] = useState(null);
    const [isHoveredCard, setIsHoveredCard] = useState(null);

    const [modeValue, setModeValue] = useState(api.squares_to_win);
    const [sizeValue, setSizeValue] = useState(api.size);

    const colorTheme1 = {
        color1_x: 'rgb(247, 101, 101)',
        color2_x: 'rgb(179, 37, 37)',
        
        color1_o: 'rgb(110, 132, 255)',
        color2_o: 'rgb(40, 50, 160)',

        color1_tie: 'rgb(92, 92, 92)',
        color2_tie: 'rgb(61, 61, 61)',

    };

    const colorTheme2 = {
        color1_x: 'rgb(68, 61, 156)',
        color2_x: 'rgb(9, 0, 94)',
        
        color1_o: 'rgb(245, 166, 92)',
        color2_o: 'rgb(226, 121, 0)',

        color1_tie: 'rgb(92, 92, 92)',
        color2_tie: 'rgb(61, 61, 61)',
    }

    const colorTheme3 = {
        color1_x: 'rgb(177, 177, 177)',
        color2_x: 'rgb(0, 0, 0)',
        
        color1_o: 'rgb(121, 0, 0)',
        color2_o: 'rgb(95, 0, 0)',

        color1_tie: 'rgb(92, 92, 92)',
        color2_tie: 'rgb(61, 61, 61)',
    }

    const colorThemes = [colorTheme1, colorTheme2, colorTheme3];
    const [currentColorTheme, setCurrentColorTheme] = useState(colorTheme1);
    const [playboardBg, setPlayboardBg] = useState(currentColorTheme.color2_x);
    const [pageBg, setPageBg] = useState(currentColorTheme.color1_x);

    useEffect(() => {

        api.gameReset();
        api.cpu_opponent = false;
        api.size = 3;
        api.squares_to_win = 3;

        setSizeValue(api.size);
        setModeValue(api.squares_to_win);
        setPlayer(api.player);

    }, []);

    useEffect(() => {

        api.gameReset();
        setPlayer(api.player);
        setWinner(null);

    }, [modeValue, sizeValue])

    useEffect(() => {

        if (winner === null) {
            setPlayer(api.player);
        } else {
            setPlayer(winner);
        }

        if (player === "O" && winner !== "tie") {
        
            setPlayboardBg(currentColorTheme.color2_o);
            setPageBg(currentColorTheme.color1_o);
        
        } else if (player === "X" && winner !== "tie") {

            setPlayboardBg(currentColorTheme.color2_x);
            setPageBg(currentColorTheme.color1_x);
        
        } else if (winner === "tie") {
            
            setPlayboardBg(currentColorTheme.color2_tie);
            setPageBg(currentColorTheme.color1_tie);
        
        }

    }, [player, currentColorTheme, winner]);

    function handleClick(row, column) {
        

        let next = api.makeMove(row, column);
        if (next === "X") {

            setWinner("X");
            setPlayboardBg(currentColorTheme.color2_x);
            setPageBg(currentColorTheme.color1_x);

        } else if (next === "O") {
        
            setWinner("O");
            setPlayboardBg(currentColorTheme.color2_o);
            setPageBg(currentColorTheme.color1_o);

        } else if (next === "tie"){

            setPlayboardBg(currentColorTheme.color2_tie);
            setPageBg(currentColorTheme.color1_tie);
            setWinner("tie");
            
        } else {

            setPlayer(api.player);
        }

    }

    return (
        <>  
            <Menu   currentColorTheme={currentColorTheme}
                    setColorTheme={setCurrentColorTheme} 
                    colorThemes={colorThemes} 
                    winner={winner} 
                    setWinner={setWinner} 
                    setPlayer={setPlayer} 
                    player={player}
                    mode={modeValue}
                    size={sizeValue}
                    setModeValue={setModeValue}
                    setSizeValue={setSizeValue}
                    
                />
            <div className="game-screen"
                style={{
                    position: 'absolute',
                    height: '100%',
                    width: '100%',
                    display: 'flex',
                    alignItems: 'center',
                    justifyContent: 'center',
                    flexDirection: 'column',
                    backgroundColor: pageBg,
                    transition: 'background-color 400ms ease',
                    zIndex: '1',
                    overflow: 'hidden'
                }}  
            >
                <FontAwesomeIcon icon={faUserGroup} style={{position: 'absolute', right: '4%', top: '5%', scale: '5', color: 'white'}}/>
                {
                    (winner !== null) ?
                    <AnimatedContent
                    distance={50}
                    direction="vertical"
                    reverse={true}
                    duration={0.9}
                    ease="bounce.out"
                    initialOpacity={0}
                    animateOpacity
                    scale={2}
                    threshold={0.1}
                    delay={0}
                    >
                        <div style={{ 
                            position: 'relative',
                            border: '2px solid transparent', 
                            width: '500px', 
                            height: '100px',
                            color: 'white',
                            fontSize: '30px',
                            fontFamily: 'Helvetica',
                            display: 'flex',
                            alignItems: 'center',
                            justifyContent: 'center',
                            WebkitTextStroke: '2px black',
                            zIndex: '3'
                            
                        }}>
                            <h1>{(winner !== "tie") ? "Vyhral si!" : "Remíza"}</h1>
                        </div>
                    </AnimatedContent>
                    :  <div style={{  
                        width: '500px', 
                        height: '100px',
                        justifySelf: 'flex-start',
                        }}>
                        </div>
                } 
                <AnimatedContent
                distance={1000}
                direction="vertical"
                reverse={true}
                duration={1.1}
                ease="elastic.out(1,0.3)"
                initialOpacity={0}
                animateOpacity
                scale={1}
                threshold={0.1}
                delay={0}
                >
                    <div className="player-icon">
                        <FontAwesomeIcon style={{
                            position: 'absolute',
                            scale: (player === "X") ? '70' : '80',
                            left: '50%',
                            top: '50%',
                            color: (player === "X") ? currentColorTheme.color1_x : currentColorTheme.color1_o,
                            transition: 'all 200ms ease',
                        }} 
                        icon={(player === "X" && winner === null)
                            ? faXmark 
                            : (player === "O" && winner === null)
                                ? faO 
                                : (winner === "X")
                                    ? faXmark
                                    : (winner === "O")
                                        ? faO
                                        : ''
                        }           
                                
                        />
                    </div>       
                    <div className="playboard" style={{

                        width: '500px',
                        height: '500px',
                        borderRadius: '10px',
                        backgroundColor: playboardBg,
                        boxShadow:  '0 0 150px white',
                        display: 'grid',
                        gridTemplateColumns: `repeat(${api.size}, 1fr)`,
                        gridTemplateRows: `repeat(${api.size}, 1fr)`,
                        gap: '1%',
                    }}
                    >
                        {Array.from({ length: api.size*api.size }).map((_, i) => {

                                const row = Math.floor(i / api.size);
                                const col = i % api.size;
        
                                const isWinningSquare = api.winning_coords.some(
                                    ([r, c]) => r === row && c === col
                                );
                                return (
                                    <div
                                    key={i}
                                        style={{
                                            backgroundColor: isWinningSquare ? (winner === "X") ? currentColorTheme.color1_x : currentColorTheme.color1_o : 'white',
                                            display: "flex",
                                            alignItems: "center",
                                            justifyContent: "center",
                                            border: isWinningSquare ? '' : '1px solid black',
                                            transition: 'all 200ms ease',
                                            borderRadius: `${20 - api.size}px`,
                                            scale: (isHoveredCard === i+1 || isWinningSquare) ? '1.05' : '1',
                                            cursor: 'pointer',
                                            fontSize: '50px',
                                        }}
                                    onMouseEnter={() => setIsHoveredCard(i+1)}
                                    onMouseLeave={() => setIsHoveredCard(null)}
                                    onClick={() => handleClick(Math.floor(i/api.size), i%api.size)}
                                    >
                                        {

                                        (api.getBoard()[Math.floor(i/api.size)][i%api.size] === "X")
                                        ? <FontAwesomeIcon style={{color: currentColorTheme.color2_x, height: '50%', width: '50%'}}icon={faXmark} />
                                        : (api.getBoard()[Math.floor(i/api.size)][i%api.size] === "O") ? <FontAwesomeIcon style={{color: currentColorTheme.color2_o, height: '50%', width: '50%'}} icon={faO}/> : null
                                    
                                        }
                                    </div>
                                )
                    
                        })}
                </div>

            </AnimatedContent>

                    

            </div>
            {
                (winner !== null) 
                ? <div style={{
                    position: 'absolute',
                    width: '100%',
                    height: '100%', 
                    display: 'flex',
                    flexDirection: 'column',
                    paddingTop: '50px',
                    alignItems: 'center',
                    justifyContent: 'center',
                    zIndex: '1',
                    }}>
                        <PlayboardBlocker winner={winner} colorTheme={currentColorTheme} />
                    </div>
                : ''        
            }
        </>
    )
}


export default TicTacToe2;