﻿using CS3505;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ClientGUI
{
    public partial class ClientLogIn : Form
    {
        /// <summary>
        /// Used to connect to the server
        /// </summary>
        private SpreadsheetController ssController;

        public ClientLogIn()
        {
            InitializeComponent();

            //It is okay to set this controller's Spreadsheet to null because it won't be 
            //doing anything spreadsheet related. It will just try to connect to the server. 
            ssController = new SpreadsheetController(null);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Verifies if the username/password combination from UsernameTextBox
        /// and PasswordTextBox is valid. If it is, open a new window listing
        /// available spreadsheets. Otherwise, print an error.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void LogInButton_Click(object sender, EventArgs e)
        {
            //3 strings. Username, Password, Address
            //Send the strings to the server to evaluate
            //if (String.IsNullOrWhiteSpace(AddressTextBox.Text))
            //{
            //    //use ssController to connect with our address string
            //}
            //ssController.Connect(AddressTextBox.Text, UsernameTextBox.Text, PasswordTextBox.Text);

            //If there is no address, default connect to Generics Spreadsheet Server.
            //Otherwise connect to Spreadsheet Server corresponding to that address.

            //If the username wasn't recognized, creates a new user with given password.
            //Successful logins open a new window listing available spreadsheets to edit.

            //If username was recognized but password was not, write an error message.
            MessageBox.Show("Username/Password Combination incorrect.",
                            "Invalid Username/Password Combination",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);

        }
    }
}
