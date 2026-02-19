
export class piskvorky {

    public playboard: any[][];
    public player: string;
    public size: number;
    public squares_to_win: number;
    public winning_coords: any[][];

    public cpu_opponent: boolean = false;

    constructor(size: number, squares_to_win: number) {
        this.size = size;
        this.squares_to_win = squares_to_win;
        this.winning_coords = Array(this.squares_to_win).fill([null,null]);
        this.gameReset();
        
    }

    // reset boardy, na rade je hrac X
    // for loop pre rozne rozmery hracej plochy
    public gameReset(): void {
        this.player = "X";
        this.playboard = Array(this.size);

        for (let row = 0; row < this.size; row++) {
            this.playboard[row] = Array(this.size).fill(null);
        }

        for (let i = 0; i < this.squares_to_win; i++) {
            this.winning_coords[i] = [null,null];
        }

    }

    // vracia aktualny stav hracej plochy (2d pole)
    public getBoard(): any {
        return this.playboard;
    }

    // kontrola remizy, ak je nejake pole este nevyplnene, vrati false
    // prerobit pre rozne 'size' hodnoty
    private checkTie(): boolean {
        for (let r = 0; r < this.size; r++) {
            for (let c = 0; c < this.size; c++) {
                if (!this.playboard[r][c]) {
                    return false;
                }
            }
        }
        for (let i = 0; i < this.squares_to_win; i++) {
            this.winning_coords[i] = [null,null];
        }
        return true;
    }

    // vrati string s oznacenim hraca, null ak sa este nedohralo
    // 
    private checkLine(row: number, col: number) {

        if (col + this.squares_to_win > this.size) {
            return null;
        }

        let tmp = Array(this.squares_to_win);
        for (let i = 0; i < this.squares_to_win; i++) {

            tmp[i] = this.playboard[row][col+i];
            this.winning_coords[i] = [row, col+i];

        }
        if (tmp.includes(null)) {
            return null;
        } else {

            let first = tmp[0];
            for (let i = 0; i < this.squares_to_win; i++) {
                if (tmp[i] !== first) {
                    return null;
                }
            }
            //console.log(this.winning_coords);
            return first;

        }

    }

    private checkColumn(row: number, col: number) {

        if (row + this.squares_to_win > this.size) {
            return null;
        }

        let tmp = Array(this.squares_to_win).fill(null);
        for (let i = 0; i < this.squares_to_win; i++) {

            if (row + i >= this.size || !this.playboard[row + i]) {
                alert("??");
            }
            tmp[i] = this.playboard[row+i][col];
            this.winning_coords[i] = [row+i, col];

        }
        if (tmp.includes(null)) {
            return null;
        } else {

            let first = tmp[0];
            for (let i = 0; i < this.squares_to_win; i++) {
                if (tmp[i] !== first) {
                    return null;
                }
            }
            //console.log(this.winning_coords);
            return first;

        }

    }

    private checkDiagonalUp(row: number, col: number) {

        if (col + this.squares_to_win > this.size) {
            return null;
        }

        if (row - (this.squares_to_win - 1) < 0 ) {
            return null;
        }


        let tmp = Array(this.squares_to_win);
        for (let i = 0; i < this.squares_to_win; i++) {

            tmp[i] = this.playboard[row-i][col+i];
            this.winning_coords[i] = [row-i, col+i];

        }
        if (tmp.includes(null)) {
            return null;
        } else {

            let first = tmp[0];
            for (let i = 0; i < this.squares_to_win; i++) {
                if (tmp[i] !== first) {
                    return null;
                }
            }
            //console.log(this.winning_coords);
            return first;

        }   

    }

    private checkDiagonalDown(row: number, col: number) {

        if (col + this.squares_to_win > this.size) {
            return null;
        }

        if (row + this.squares_to_win > this.size ) {
            return null;
        }


        let tmp = Array(this.squares_to_win);
        for (let i = 0; i < this.squares_to_win; i++) {

            tmp[i] = this.playboard[row+i][col+i];
            this.winning_coords[i] = [row+i, col+i];

        }
        if (tmp.includes(null)) {
            return null;
        } else {

            let first = tmp[0];
            for (let i = 0; i < this.squares_to_win; i++) {
                if (tmp[i] !== first) {
                    return null;
                }
            }
            console.log(this.winning_coords);
            return first;

        }   

    }

    private checkWinner(): string|null {
        for (let i = 0; i < this.squares_to_win; i++) {
            this.winning_coords[i] = [null, null];
        }
        let tmp = null;
        for (let i = 0; i < this.size; i++) {
            for (let j = 0; j < this.size; j++) {

                tmp = this.checkLine(i, j);
                if (tmp) return tmp;

                tmp = this.checkColumn(i,j);
                if (tmp) return tmp;

                tmp = this.checkDiagonalDown(i, j);
                if (tmp) return tmp;

                tmp = this.checkDiagonalUp(i,j);
                if (tmp) return tmp;

            } 
        }

        if (this.checkTie()) return "tie";

        for (let i = 0; i < this.squares_to_win; i++) {
            this.winning_coords[i] = [null,null];
        }
        

        return tmp;

    }

    private makeCpuMove(): string|null {

        let emptySquares: Array<{row: number, col: number}> = [];
        for (let r = 0; r < this.size; r++) {
            for (let c = 0; c < this.size; c++) {
                if (!this.playboard[r][c]) {
                    emptySquares.push({row: r, col: c});
                }
            }
        }
        if (emptySquares.length > 0 && this.player === "O") {
            let choice = emptySquares[Math.floor(Math.random() * emptySquares.length)];
            this.playboard[choice.row][choice.col] = "O";
            this.player = "X";
        }

        return this.checkWinner();
    }

    // po kazdom oznaceni policka na hracej ploche by sa mala volat tato funkcia
    public makeMove(row: number, col: number): string|null  {

        if (!this.playboard[row][col]) {

            this.playboard[row][col] = this.player;

            if (this.player === "X") {
                this.player = "O";
            } else {
                this.player = "X";
            }
        }

        return this.checkWinner();
    }
}