using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ICS_Project.DAL.Migrations
{
    /// <inheritdoc />
    public partial class UnitOfWorkUpdate : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_TimeAddeds_MultimediaFiles_MultimediaFileid",
                table: "TimesAdded");

            migrationBuilder.RenameColumn(
                name: "MultimediaFileid",
                table: "TimesAdded",
                newName: "MultimediaFileId");

            migrationBuilder.RenameColumn(
                name: "id",
                table: "TimesAdded",
                newName: "Id");

            migrationBuilder.RenameIndex(
                name: "IX_TimeAddeds_MultimediaFileid",
                table: "TimesAdded",
                newName: "IX_TimeAddeds_MultimediaFileId");

            migrationBuilder.RenameColumn(
                name: "id",
                table: "Playlists",
                newName: "Id");

            migrationBuilder.RenameColumn(
                name: "id",
                table: "MultimediaFiles",
                newName: "Id");

            migrationBuilder.AddColumn<string>(
                name: "PictureUrl",
                table: "Playlists",
                type: "TEXT",
                nullable: true);

            migrationBuilder.AlterColumn<string>(
                name: "PictureUrl",
                table: "MultimediaFiles",
                type: "TEXT",
                nullable: true,
                oldClrType: typeof(string),
                oldType: "TEXT");

            migrationBuilder.AlterColumn<string>(
                name: "Description",
                table: "MultimediaFiles",
                type: "TEXT",
                nullable: true,
                oldClrType: typeof(string),
                oldType: "TEXT");

            migrationBuilder.AddForeignKey(
                name: "FK_TimeAddeds_MultimediaFiles_MultimediaFileId",
                table: "TimesAdded",
                column: "MultimediaFileId",
                principalTable: "MultimediaFiles",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_TimeAddeds_MultimediaFiles_MultimediaFileId",
                table: "TimesAdded");

            migrationBuilder.DropColumn(
                name: "PictureUrl",
                table: "Playlists");

            migrationBuilder.RenameColumn(
                name: "MultimediaFileId",
                table: "TimesAdded",
                newName: "MultimediaFileid");

            migrationBuilder.RenameColumn(
                name: "Id",
                table: "TimesAdded",
                newName: "id");

            migrationBuilder.RenameIndex(
                name: "IX_TimeAddeds_MultimediaFileId",
                table: "TimesAdded",
                newName: "IX_TimeAddeds_MultimediaFileid");

            migrationBuilder.RenameColumn(
                name: "Id",
                table: "Playlists",
                newName: "id");

            migrationBuilder.RenameColumn(
                name: "Id",
                table: "MultimediaFiles",
                newName: "id");

            migrationBuilder.AlterColumn<string>(
                name: "PictureUrl",
                table: "MultimediaFiles",
                type: "TEXT",
                nullable: false,
                defaultValue: "",
                oldClrType: typeof(string),
                oldType: "TEXT",
                oldNullable: true);

            migrationBuilder.AlterColumn<string>(
                name: "Description",
                table: "MultimediaFiles",
                type: "TEXT",
                nullable: false,
                defaultValue: "",
                oldClrType: typeof(string),
                oldType: "TEXT",
                oldNullable: true);

            migrationBuilder.AddForeignKey(
                name: "FK_TimeAddeds_MultimediaFiles_MultimediaFileid",
                table: "TimesAdded",
                column: "MultimediaFileid",
                principalTable: "MultimediaFiles",
                principalColumn: "id",
                onDelete: ReferentialAction.Restrict);
        }
    }
}
