from django.db import models


class User(models.Model):
    name = models.CharField(max_length=30)
    username = models.CharField(max_length=30, null=False, blank=False, unique=True)
    password = models.CharField(max_length=20, null=False, blank=False)
    status = models.BooleanField(default=False)  # online or offline
    created_at = models.DateTimeField(auto_now_add=True)
    is_admin = models.BooleanField(default=False)


class Friends(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    friend = models.IntegerField(null=True)
    confirm = models.BooleanField(default=False)

    class Meta:
        unique_together = ('user', 'friend')

    # neu = False : doi tra loi, neu = True ban be
    # neu khong dong y : Xoa luon ban ghi


class Room(models.Model):
    name = models.CharField(max_length=20, default=None, null=True)
    member = models.ManyToManyField(User)
    admin_room = models.IntegerField()


class Message(models.Model):
    room = models.ForeignKey(Room, on_delete=models.CASCADE)
    from_user = models.IntegerField(null=False)
    message = models.TextField()
    created_at = models.DateTimeField(auto_now_add=True)

    # select * from message where to_id = id dang nhap


#
class ChatPrivate(models.Model):
    from_user = models.IntegerField(null=False)
    message = models.TextField()
    created_at = models.DateTimeField(auto_now_add=True)
    to_user = models.ForeignKey(User, on_delete=models.CASCADE)
