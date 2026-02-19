

import { useEffect, useState } from "react";
import { api } from "./ApiInstance.ts";

function PlayboardBlocker({ winner, colorTheme}) {
    return (
        <>
                    <div className="blocker" style={{
                        width: '500px',
                        height: '500px',
                        borderRadius: '10px',
                        transition: 'all 200ms ease',
                        backgroundColor: 'transparent',
                        boxShadow:  '0 0 150px white',
                        display: 'grid',
                        gridTemplateColumns: `repeat(${api.size}, 1fr)`,
                        gridTemplateRows: `repeat(${api.size}, 1fr)`,
                        gap: '5px'
                    }}
                    >
                    </div>    
     
        </>
    )
}

export default PlayboardBlocker;