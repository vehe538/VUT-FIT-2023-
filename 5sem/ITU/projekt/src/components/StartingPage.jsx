import { useState ,useEffect} from "react";
import { useNavigate } from "react-router-dom";
import { api } from "./ApiInstance.ts";

import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faUserGroup, faUser} from '@fortawesome/free-solid-svg-icons'

function StartingPage() {
  const [isHoveredOne, setIsHoveredOne] = useState(false);
  const [isHoveredTwo, setIsHoveredTwo] = useState(false);

  const colorOne = isHoveredOne ? 'rgb(245, 234, 84)' : 'rgba(247, 231, 3, 0.2)';
  const colorTwo = isHoveredTwo ? 'rgb(248, 198, 105)' : 'rgba(247, 162, 3, 0.31)';

  let navigate = useNavigate();

  useEffect(() => {
    api.gameReset();
  });


  return (
    <div
      className="welcome-screen"
      style={{
        display: 'flex',
        height: '100vh',
        width: '100vw',
        transition: 'all 200ms ease',
      }}
    >
      <div
        className="one-player-option"
        style={{
          flex: isHoveredOne ? 1.2 : 1,
          zIndex: isHoveredOne ? '1' : '',
          height: '100%',
          backgroundColor: colorOne,
          display: 'flex',
          flexDirection: 'column',
          justifyContent: 'space-evenly',
          alignItems: 'center',
          transition: 'flex 400ms ease, background-color 400ms ease, transform 300ms ease, font-size 300ms ease, color 200ms ease',
          cursor: 'pointer',
          fontSize: isHoveredOne ? '250%' : '200%',
          fontWeight: isHoveredOne ? '600' : '500',
          color: !isHoveredOne ? 'rgb(204, 173, 0)' : 'black',
        }}
        onMouseEnter={() => setIsHoveredOne(true)}
        onMouseLeave={() => setIsHoveredOne(false)}
        onClick={() => navigate("/tictactoe1p")}
      >
        <p>Jeden hráč</p>
        <FontAwesomeIcon icon={faUser} style={{scale: '8', transition: 'all 200ms ease'}}/>
      </div>

      <div
        className="two-player-option"
        style={{
          flex: isHoveredTwo ? 1.2 : 1,
          zIndex: isHoveredTwo ? '1' : '',
          height: '100%',
          backgroundColor: colorTwo,
          display: 'flex',
          flexDirection: 'column',
          justifyContent: 'space-evenly',
          alignItems: 'center',
          transition: 'flex 400ms ease, background-color 400ms ease, transform 300ms ease, font-size 300ms ease, color 200ms ease',
          cursor: 'pointer',
          fontSize: isHoveredTwo ? '250%' : '200%',
          fontWeight: isHoveredTwo ? '600' : '500',
          color: !isHoveredTwo ? 'rgb(194, 137, 32)' : 'black',

        }}
        onMouseEnter={() => setIsHoveredTwo(true)}
        onMouseLeave={() => setIsHoveredTwo(false)}
        onClick={() => navigate("/tictactoe2p")}
      >
        <p>Dvaja hráči</p>
        <FontAwesomeIcon icon={faUserGroup} style={{scale: '8', transition: 'all 200ms ease'}}/>
      </div>

        <div className="title"
            style={{
                position: 'absolute',
                zIndex: '2',
                height: '100px',
                width: '30%',
                left: isHoveredTwo ? '58%' : isHoveredOne ? '12%' : '35%',
                top: '0%',
                boxShadow: '0 80px 140px white',
                borderRadius: '20px',
                display: 'flex',
                justifyContent: 'center',
                alignItems: 'center',
                fontWeight: '500',
                fontSize: '80px',
                fontFamily: 'ubuntu mono',
                transition: 'left 500ms ease, right 500ms ease',
                color: 'black',
                backgroundColor: isHoveredTwo ? 'rgb(248, 198, 105)' : isHoveredOne ? 'rgb(245, 234, 84)' : 'transparent'
                
                
            }}
        >
          Piškvorky
        </div>
    </div>
  );
}

export default StartingPage;
