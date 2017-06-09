/**
 * Created by root on 6/3/17.
 */
$(function() {
    Vue.component('note', {
        template: '<div class="note uk-width" style="width: 230px;">\
    <div class="uk-panel uk-panel-box">\
        <div class="uk-panel-title"><input class="input-box" type="text" v-model="title" v-on:change="autoUpdate" style="width: 120px" placeholder="empty">\
            <a class="uk-icon-justify uk-icon-close uk-float-right" v-on:click="remove"></a></div>\
        <textarea class="input-box" v-model="content" v-on:change="autoUpdate" style="height: 200px;resize: none;" placeholder="empty"></textarea>\
    </div>\
</div>',
        props: ["note", "index"],
        data: function () {
            return this.note;
        },
        mounted: function () {
            var jdom = $(this.$el).find(".input-box");
            $(this.$el).find(".uk-panel").css("background-color", "#93E0FF").css("border-radius", "5px");
            jdom.css("border", "none");
            jdom.css("background-color", "#93E0FF");
            jdom.attr("readonly", "readonly");
            jdom.click(function () {
                $(this).attr("readonly", false);
                $(this).css("background-color", "#ffffff");
            });
            jdom.blur(function () {
                $(this).attr("readonly", "readonly");
                $(this).css("background-color", "#93E0FF");
            });
        },
        methods: {
            remove: function () {
                this.$emit('remove', this.index);
            },
            autoUpdate: function () {
                var data = {
                    nid: this.note.nid,
                    title: this.note.title,
                    content: this.note.content
                };

                $.post("/update", data = JSON.stringify(data), function (data, status) {

                })
            }
        }
    })

    var vm = new Vue({
        el: "#app",
        data: function () {
            return {
                notes: [],
                nid: 0,
                isSignIn: false,
                username: ""
            }
        },
        created: function () {
            var self = this;
            //验证cookie有效性
            var token = $.cookie("token");
            var uid = $.cookie("UID");
            if (token != undefined && token != "") {
                $.post("/verity", {}, function (data, statue) {
                    if (data.status) {
                        self.username = data.username;
                        self.isSignIn = true;
                        self.fetchAllNotes();
                    } else {
                        self.isSignIn = false;
                        self.username = "";
                    }
                })
            } else {
                this.isSignIn = false;
                self.username = "";
            }

        },
        methods: {
            signup: function () {
                var self = this;
                var username = $('.form-signup').find('input[name=username]').val();
                var password = $('.form-signup').find('input[name=password]').val();
                var cpassword = $('.form-signup').find('input[name=cpassword]').val();
                if (username.match(/[a-zA-Z0-9]+/) == null || username.match(/[a-zA-Z0-9]+/)[0] !== username || password == '' || password.length < 8 || password != cpassword) {
                    return;
                } else {
                    $.post("/signup", {
                        username: username,
                        password: password
                    }, function (data, status) {
                        if (data.status) {
                            var modal = UIkit.modal("#signup-modal");
                            modal.hide();
                            self.username = username;
                            self.isSignIn = true;
                            UIkit.notify("注册成功");
                        } else {
                            UIkit.notify("注册失败");
                        }
                    });
                }
            },
            signin: function () {
                var self = this;
                var username = $('.form-signin').find('input[name=account]').val();
                var password = $('.form-signin').find('input[name=password]').val();
                if (username.trim() == '' || password.trim() == '') {
                    return;
                } else {
                    $.post('/signin', {
                        username: username,
                        password: password
                    }, function (data, status) {
                        if (data.status == true) {
                            var modal = UIkit.modal("#signin-modal");
                            modal.hide();
                            self.username = username;
                            self.isSignIn = true;
                            self.fetchAllNotes();
                            UIkit.notify("登录成功");
                        } else {
                            UIkit.notify("登录失败");
                        }
                    })
                }
            },
            signout: function () {
                var self = this;
                $.post('/signout', {}, function (data, status) {
                    if (data.status == true) {
                        self.username = "";
                        self.uid = 0;
                        self.isSignIn = false;
                        self.notes = [];
                        window.location = "/";
                    } else {
                    }
                })
            },
            fetchAllNotes: function () {
                var self = this;
                var data = {
                    nid: 0
                };
                $.post("/fetch", data = JSON.stringify(data), function (data, status) {
                    if (data.status == true) {
                        for (var i = 0; i < data.notes.length; i++) {
                            self.notes.push(data.notes[i]);
                        }
                        UIkit.notify("获取完成", {timeout: 500});
                    }
                })
            },
            createNote: function (event) {
                //Ïò·þÎñÆ÷·¢ËÍÇëÇó
                //post
                var self = this;
                var data = {
                    title: "",
                    content: ""
                };

                $.post("/create", data = JSON.stringify(data), function (data, status) {
                    if (data.status == true) {
                        var noteInfo = {
                            title: "",
                            content: "",
                            nid: data.nid
                        }
                        self.notes.unshift(noteInfo)
                    }
                })

            },
            deleteNote: function (index) {
                var self = this;
                var data = {
                    nid: this.notes[index].nid
                };
                $.post('/delete', data = JSON.stringify(data), function (data, status) {
                    if (data.status == true) {
                        self.notes.splice(index, 1);
                    }
                })

            }
        }
    })

    function checkPassword() {
        var password = $('.form-signup input[name=password]').val();
        var cpassword = $('.form-signup input[name=cpassword]').val();
        if(password.length < 8){
            $('.cp-alert').empty();
            $('.cp-alert').append(`<div class="alert alert-danger">
                        <strong>密码长度至少8位</strong>
                    </div>`);
            return;
        }

        if (cpassword == '') {
            $('.cp-alert').empty();
            return;
        }
        if (password != cpassword) {
            $('.cp-alert').empty();
            $('.cp-alert').append(`<div class="alert alert-danger">
                        <strong>密码不一致</strong>
                    </div>`)
        } else {
            $('.cp-alert').empty();
        }
    }
    $('.form-signup input[name=password]').change(checkPassword);
    $('.form-signup input[name=cpassword]').change(checkPassword);

    function checkUname() {
        var username = $('.form-signup input[name=username]').val();
        if(username.match(/[a-zA-Z0-9]+/) == null || username.match(/[a-zA-Z0-9]+/)[0] !== username){
            $('.n-alert').empty();
            $('.n-alert').append(`<div class="alert alert-danger">
                        <strong>用户名只能由数字和字母组成</strong>
                    </div>`)
            return;
        }else{
            $('.n-alert').empty();
        }

        $.post('/checkuname', {
            username: username
        }, function (data, status) {

            if (data.status) {
                $('.n-alert').empty();
            } else {
                $('.n-alert').empty();
                $('.n-alert').append(`<div class="alert alert-danger">
                        <strong>用户名已存在</strong>
                    </div>`)
            }
        });
    }


    $('.form-signup input[name=username]').change(checkUname);
})