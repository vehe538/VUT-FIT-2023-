import { useState, useEffect } from "react";
//import { useNavigate } from "react-router-dom";

import { api } from "./ApiInstance.ts";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faXmark, faO, faUserGroup} from '@fortawesome/free-solid-svg-icons'
import MenuButton from "./MenuButton.jsx";
import Menu from "./Menu.jsx";


function Test() {
    const [player, setPlayer] = useState(null);
    const [winner, setWinner] = useState(null);
    const [isHoveredCard, setIsHoveredCard] = useState(null);

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

    return (
        <>
            <div style={{
                backgroundColor: 'gray',
                height: '1000px',
                width: '100%',
                border: '2px solid red'
            }}>
                <Menu currentColorTheme={currentColorTheme} setColorTheme={setCurrentColorTheme} colorThemes={colorThemes} winner={winner} setWinner={setWinner} setPlayer={setPlayer} player={player}/>
            </div>
        </>
    )
}

export default Test;